// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "page.h"
#include "pmm.h"
#include <corn_os/algorithm.h>
#include <corn_libc/stdio.h>
#include <corn_libc/stddef.h>
#include <corn_libc/string.h>
#include "pm_manager/pm_manager.h"

/**
 * get_pte - get page table entry and return the kernel virtual address of this pte for la
 *         - if the PT contains this pte didn't exist, alloc a page for PT
 *
 * @pgdir:  the kernel virtual base address of PDT
 * @la:     the linear address need to map
 * @create: whether alloc a page for PT
 *
 * @return: the kernel virtual address of this pte
 */
pte_t *get_pte(pde_t *pgdir, uintptr_t la, _Bool create_page_table)
{
	pde_t *pdep = &pgdir[page_dir_index(la)];
	if (!(*pdep & PTE_Present)) {
		struct Page *page;
		if (!create_page_table ||
		    (page = pm_manager->alloc_pages(1)) == NULL) {
			return NULL;
		}
		set_page_ref(page, 1);
		uintptr_t pa = page2pa(page);

		// clear page content
		memset((void *)kern_virtual_addr(pa), 0, PGSIZE);

		// set page directory entry's permission
		*pdep = pa | PTE_User | PTE_Writeable | PTE_Present;
	}
	return &((pte_t *)kern_virtual_addr(PDE_ADDR(*pdep)))[PTX(la)];
}

void page_init()
{
	struct e820map *memmap =
		(struct e820map *)kern_virtual_addr(MEMMAP_ADDR);

	puts("e820map:");
	uint64_t max_physical_addr = 0;
	for (int i = 0; i < memmap->nr_map; ++i) {
		uint64_t begin = memmap->map[i].addr,
			 end = begin + memmap->map[i].size;
		printf("memory area %x: %llx, [%llx, %llx], type = %lx.\n", i,
		       memmap->map[i].size, begin, end - 1,
		       memmap->map[i].type);
		if (memmap->map[i].type == E820_ADDR_RANGE_MEM &&
		    max_physical_addr < end && begin < KMEMSIZE) {
			max_physical_addr = end;
		}
	}
	max_physical_addr = MIN(max_physical_addr, KMEMSIZE);
	num_pages = max_physical_addr / PGSIZE;

	// defined in tools/kernel.ld
	extern char kernel_end[];
	pages = (struct Page *)ROUNDUP((void *)kernel_end, PGSIZE);
	printf("max_physical_addr: %llx; num_pages: %u\n", max_physical_addr,
	       num_pages);

	for (int i = 0; i < num_pages; ++i) {
		set_page_reserved(pages + i);
	}

	uintptr_t free_mem = kern_physical_addr(
		(uintptr_t)pages + sizeof(struct Page) * num_pages);
	for (int i = 0; i < memmap->nr_map; ++i) {
		uint64_t begin = memmap->map[i].addr,
			 end = begin + memmap->map[i].size;
		if (memmap->map[i].type == E820_ADDR_RANGE_MEM) {
			begin = ROUNDUP(MAX(begin, free_mem), PGSIZE);
			end = ROUNDDOWN(MIN(end, KMEMSIZE), PGSIZE);
			if (begin < end) {
				pm_manager->init_memmap(pa2page(begin),
							(end - begin) / PGSIZE);
			}
		}
	}
}
