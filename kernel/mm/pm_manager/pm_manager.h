// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_KERNEL_MM_PM_MANAGER_PM_MANAGER_H
#define CORNOS_KERNEL_MM_PM_MANAGER_PM_MANAGER_H

#include "../page.h"
#include <corn_os/list.h>

#define EXPORT_PMM_MANAGER(name)                              \
	struct PmManager name = { #name,                      \
				  .init = init,               \
				  .init_memmap = init_memmap, \
				  .alloc_pages = alloc_pages, \
				  .free_pages = free_pages,   \
				  .num_free_pages = num_free_pages };

// convert list head to page
#define list_head2page(le, member) to_struct((le), struct Page, member)

/* free_area_t - maintains a doubly linked list to record free (unused) pages */
typedef struct {
	struct list_head free_list; // the list header
	unsigned int num_free; // number of free pages in this free list
} free_area_t;

struct PmManager {
	const char *name;
	void (*init)();

	/** initialize memory with @n pages */
	void (*init_memmap)(struct Page *base, size_t n);

	/* allocate >=n pages, depend on the allocation algorithm */
	struct Page *(*alloc_pages)(size_t n);

	// free >=n pages with "base" addr of Page descriptor structures(layout.h)
	void (*free_pages)(struct Page *base, size_t n);
	size_t (*num_free_pages)();
};

extern struct PmManager *pm_manager;

void pm_manager_init();

#endif // CORNOS_KERNEL_MM_PM_MANAGER_PM_MANAGER_H
