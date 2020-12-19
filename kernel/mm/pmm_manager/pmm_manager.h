// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_KERNEL_MM_PMM_MANAGER_PMM_MANAGER_H
#define CORNOS_KERNEL_MM_PMM_MANAGER_PMM_MANAGER_H

#include "../pmm.h"
#include <corn_libc/stddef.h>

#define EXPORT_PMM_MANAGER(name)                               \
	struct PmmManager name = { #name,                      \
				   .init = init,               \
				   .init_memmap = init_memmap, \
				   .alloc_pages = alloc_pages, \
				   .free_pages = free_pages,   \
				   .num_free_pages = num_free_pages };

struct PmmManager {
	const char *name;
	void (*init)();
	void (*init_memmap)(struct Page *base, size_t n);

	/* allocate >=n pages, depend on the allocation algorithm */
	struct Page *(*alloc_pages)(size_t n);

	// free >=n pages with "base" addr of Page descriptor structures(layout.h)
	void (*free_pages)(struct Page *base, size_t n);
	size_t (*num_free_pages)();
};

extern struct PmmManager first_fit, buddy, slub;
#endif // CORNOS_KERNEL_MM_PMM_MANAGER_PMM_MANAGER_H
