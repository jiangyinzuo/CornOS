// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "pmm.h"

#include <corn_libc/stdio.h>
#include <corn_os/algorithm.h>
#include <arch/x86.h>
#include "mmu.h"
#include "layout.h"

/* *
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
 * */
static struct taskstate ts = { 0 };

// virtual address of physicall page array
struct Page *pages;
// amount of physical memory (in pages)
uint64_t num_pages = 0;

/* *
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
 * */
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

/* temporary kernel stack */
uint8_t stack0[1024];

/* gdt_init - initialize the default GDT and TSS */
static void gdt_init(void)
{
	// Setup a TSS so that we can get the right stack when we trap from
	// user to the kernel. But not safe here, it's only a temporary value,
	// it will be set to KSTACKTOP in lab2.
	ts.ts_esp0 = (uint32_t)&stack0 + sizeof(stack0);
	ts.ts_ss0 = KERNEL_DS;

	// initialize the TSS filed of the gdt
	gdt[SEG_TSS] = SEG16(STS_T32A, (uint32_t)&ts, sizeof(ts), DPL_KERNEL);
	gdt[SEG_TSS].sd_s = 0;

	// reload all segment registers
	lgdt(&gdt_pd);

	// load the TSS
	ltr(GD_TSS);
}

void page_init()
{
	struct e820map *memmap = (struct e820map *)(MEMMAP_ADDR + KERNBASE);

	puts("e820map:");
	uint64_t max_page = 0;
	for (int i = 0; i < memmap->nr_map; ++i) {
		uint64_t begin = memmap->map[i].addr,
			 end = begin + memmap->map[i].size;
		printf("memory area %x: %llu, [%llu, %llu], type = %lu.\n", i,
		       memmap->map[i].size, begin, end - 1,
		       memmap->map[i].type);
		if (memmap->map[i].type == E820_ARM && max_page < end &&
		    begin < KMEMSIZE) {
			max_page = end;
		}
	}
	max_page = MIN(max_page, KMEMSIZE);
	num_pages = max_page / (uint64_t)PGSIZE;

	printf("max_page: %llu; num_pages: %llu\n", max_page, num_pages);
}

/* pmm_init - initialize the physical memory management */
void pmm_init()
{
	// detect physical memory space, reserve already used memory,
	// then use pmm->init_memmap to create free page list
	page_init();

	gdt_init();
}
