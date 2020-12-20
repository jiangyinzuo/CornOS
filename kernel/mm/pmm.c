// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "pmm.h"

#include <arch/x86.h>
#include "./pm_manager/pm_manager.h"
#include "mmu.h"
#include "layout.h"
#include "page.h"
#include <corn_libc/stdio.h>
#include <corn_libc/stddef.h>
#include <corn_libc/assert.h>
#include <corn_os/algorithm.h>

/*
 * Task State Segment:
 *
 * The TSS may reside anywhere in memory. A special segment register called
 * the Task Register (TR) holds a segment selector that points a valid TSS
 * segment descriptor which resides in the GDT. Therefore, to use a TSS
 * the following must be done in function gdt_init:
 *   - create a TSS descriptor entry in GDT
 *   - add enough information to the TSS in memory as needed
 *   - load the TR register with a segment selector for that segment
 *
 * There are several fileds in TSS for specifying the new stack pointer when a
 * privilege level change happens. But only the fields SS0 and ESP0 are useful
 * in our os kernel.
 *
 * The field SS0 contains the stack segment selector for CPL = 0, and the ESP0
 * contains the new ESP value for CPL = 0. When an interrupt happens in protected
 * mode, the x86 CPU will look in the TSS for SS0 and ESP0 and load their value
 * into SS and ESP respectively.
 */
static struct taskstate ts = { 0 };

// virtual address of physical page array
struct Page *pages;
// amount of physical memory (in pages)
size_t num_pages = 0;

pde_t *boot_pgdir = &__boot_pgdir;

// physical address of boot-time page directory
uintptr_t boot_cr3;

/* *
 * The page directory entry corresponding to the virtual address range
 * [VPT, VPT + PTSIZE) points to the page directory itself. Thus, the page
 * directory is treated as a page table as well as a page directory.
 *
 * One result of treating the page directory as a page table is that all PTEs
 * can be accessed though a "virtual page table" at virtual address VPT. And the
 * PTE for number n is stored in PageTableVA[n].
 *
 * A second consequence is that the contents of the current page directory will
 * always available at virtual address PGADDR(PDX(VPT), PDX(VPT), 0), to which
 * PageDirVA is set bellow.
 * */
pte_t *const PageTableVA = (pte_t *)PAGE_TABLE_VA;
pde_t *const PageDirVA = (pde_t *)PGADDR(page_dir_index(PAGE_TABLE_VA),
					 page_dir_index(PAGE_TABLE_VA), 0);

/*
 * Global Descriptor Table:
 *
 * The kernel and user segments are identical (except for the DPL). To load
 * the %ss register, the CPL must equal the DPL. Thus, we must duplicate the
 * segments for the user and the kernel. Defined as follows:
 *   - 0x0 :  unused (always faults -- for trapping NULL far pointers)
 *   - 0x8 :  kernel code segment
 *   - 0x10:  kernel data segment
 *   - 0x18:  user code segment
 *   - 0x20:  user data segment
 *   - 0x28:  defined for tss, initialized in gdt_init
 */
static struct segdesc gdt[] = {
	SEG_NULL,
	[SEG_KERNEL_TEXT] = SEG(STA_X | STA_R, 0x0, 0xFFFFFFFF, DPL_KERNEL),
	[SEG_KERNEL_DATA] = SEG(STA_W, 0x0, 0xFFFFFFFF, DPL_KERNEL),
	[SEG_USER_TEXT] = SEG(STA_X | STA_R, 0x0, 0xFFFFFFFF, DPL_USER),
	[SEG_USER_DATA] = SEG(STA_W, 0x0, 0xFFFFFFFF, DPL_USER),
	[SEG_TSS] = SEG_NULL,
};

static struct XDTR_t gdt_pd = { sizeof(gdt) - 1, (uint32_t)gdt };

/**
 * lgdt - load the global descriptor table register and reset the
 * data/code segement registers for kernel.
 */
static inline void lgdt(struct XDTR_t *pd)
{
	asm volatile("lgdt (%0)" ::"r"(pd));
	asm volatile("movw %%ax, %%gs" ::"a"(USER_DS));
	asm volatile("movw %%ax, %%fs" ::"a"(USER_DS));
	asm volatile("movw %%ax, %%es" ::"a"(KERNEL_DS));
	asm volatile("movw %%ax, %%ds" ::"a"(KERNEL_DS));
	asm volatile("movw %%ax, %%ss" ::"a"(KERNEL_DS));
	// reload cs
	asm volatile("ljmp %0, $1f\n 1:\n" ::"i"(KERNEL_CS));
}

/**
 * load_esp0 - change the ESP0 in default task state segment,
 * so that we can use different kernel stack when we trap frame
 * user to kernel.
 */
void load_esp0(uintptr_t esp0)
{
	ts.ts_esp0 = esp0;
}

/* gdt_init - initialize the default GDT and TSS */
static void gdt_init()
{
	// set boot kernel stack and default SS0
	load_esp0((uintptr_t)bootstacktop);
	ts.ts_ss0 = KERNEL_DS;

	// initialize the TSS filed of the gdt
	// SEG_TSS has changed from SEG16 since v0.2.3
	gdt[SEG_TSS] = SEGTSS(STS_T32A, (uintptr_t)&ts, sizeof(ts), DPL_KERNEL);

	// reload all segment registers
	lgdt(&gdt_pd);

	// load the TSS
	ltr(GD_TSS);
}

/**
 * boot_map_segment - setup&enable the paging mechanism
 * map all physical memory to linear memory with base linear addr KERNBASE
 * linear_addr KERNBASE ~ KERNBASE + KMEMSIZE = phy_addr 0 ~ KMEMSIZE
 */
static void boot_map_segment()
{
	// recursively insert boot_pgdir in itself
	// to form a virtual page table at virtual address PAGE_TABLE_VA
	boot_pgdir[page_dir_index(PAGE_TABLE_VA)] =
		kern_physical_addr((uintptr_t)boot_pgdir) | PTE_P | PTE_RW;

	uintptr_t la = KERNBASE, pa = 0;
	_Static_assert(
		PGOFF(KERNBASE) == PGOFF(0),
		"page offset linear addr should be equal to page offset physical addr");

	size_t _num_pages = ROUNDUP(KMEMSIZE + PGOFF(la), PGSIZE) / PGSIZE;

	la = ROUNDDOWN(la, PGSIZE);
	pa = ROUNDDOWN(pa, PGSIZE);
	for (; _num_pages-- > 0; la += PGSIZE, pa += PGSIZE) {
		pte_t *ptep = get_pte(boot_pgdir, la, 1);
		assert(ptep != NULL);
		*ptep = pa | PTE_P | PTE_RW;
	}
}

/**
 * pmm_init - initialize the physical memory management
 *
 * Before calling pmm_init, only 0~4MB virtual address are mapped to
 * physical address(va = pa + KERNBASE).
 *
 * pmm_init will map 0~KMEMSIZE virtual address and set gdt
 */
void pmm_init()
{
	// We've already enabled paging
	boot_cr3 = kern_physical_addr((uintptr_t)boot_pgdir);

	pm_manager_init();

	// detect physical memory space, reserve already used memory,
	// then use pmm->init_memmap to create free page list
	page_init();

	boot_map_segment();

	// Since we are using bootloader's GDT,
	// we should reload gdt (second time, the last time) to get user segments and the TSS
	// map virtual_addr 0 ~ 4G = linear_addr 0 ~ 4G
	// then set kernel stack (ss:esp) in TSS, setup TSS in gdt, load TSS
	gdt_init();
}

/**
 * get_pgtable_items - In [left, right] range of PDT or PT, find a continuous linear addr space
 *                  - (left_store*X_SIZE~right_store*X_SIZE) for PDT or PT
 *                  - X_SIZE=PTSIZE=4M, if PDT; X_SIZE=PGSIZE=4K, if PT
 *  @left:        no use ???
 *  @right:       the high side of table's range
 *  @start:       the low side of table's range
 *  @table:       the beginning addr of table
 *  @left_store:  the pointer of the high side of table's next range
 *  @right_store: the pointer of the low side of table's next range
 * return value: 0 - not a invalid item range, perm - a valid item range with perm permission
 */
static int get_pgtable_items(size_t left, size_t right, size_t start,
			     uintptr_t *table, size_t *left_store,
			     size_t *right_store)
{
	if (start >= right) {
		return 0;
	}
	while (start < right && !(table[start] & PTE_P)) {
		start++;
	}
	if (start < right) {
		if (left_store != NULL) {
			*left_store = start;
		}
		int perm = (table[start++] & PTE_USER);
		while (start < right && (table[start] & PTE_USER) == perm) {
			start++;
		}
		if (right_store != NULL) {
			*right_store = start;
		}
		return perm;
	}
	return 0;
}

//perm2str - use string 'u,r,w,-' to present the permission
static const char *perm2str(int perm)
{
	static char str[4] = "-r-";
	str[0] = (perm & PTE_US) ? 'u' : '-';
	str[2] = (perm & PTE_RW) ? 'w' : '-';
	return str;
}

void print_page_table()
{
	puts("-------------------- BEGIN --------------------");
	size_t left, right = 0, perm;
	while ((perm = get_pgtable_items(0, NUM_PDE, right, PageDirVA, &left,
					 &right)) != 0) {
		printf("PDE(%u) la: %08x-%08x %s\n", right - left,
		       left * PTSIZE, right * PTSIZE, perm2str(perm));
		size_t l, r = left * NUM_PTE;
		while ((perm = get_pgtable_items(left * NUM_PTE,
						 right * NUM_PTE, r,
						 PageTableVA, &l, &r)) != 0) {
			printf("  |-- PTE(%u) la: %08x-%08x %s\n", r - l,
			       l * PGSIZE, r * PGSIZE, perm2str(perm));
		}
	}
	puts("--------------------- END ---------------------");
}
