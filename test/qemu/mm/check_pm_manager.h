// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_TEST_QEMU_MM_CHECK_PM_MANAGER_H
#define CORNOS_TEST_QEMU_MM_CHECK_PM_MANAGER_H

#include "../../../include/corn_libc/stdio.h"
#include "../../../include/corn_libc/stddef.h"
#include "../../../include/corn_libc/assert.h"
#include "../../../include/corn_libc/string.h"
#include "../../../kernel/mm/pmm.h"
#include "../../../kernel/mm/pm_manager/pm_manager.h"
#include "check_pm_manager.h"

static void basic_check()
{
	struct Page *p0, *p1, *p2;
	p0 = p1 = p2 = NULL;
	assert((p0 = pm_manager->alloc_pages(1)) != NULL);
	assert((p1 = pm_manager->alloc_pages(1)) != NULL);
	assert((p2 = pm_manager->alloc_pages(1)) != NULL);

	assert(p0 != p1 && p0 != p2 && p1 != p2);
	assert(page_ref(p0) == 0 && page_ref(p1) == 0 && page_ref(p2) == 0);

	assert(page2pa(p0) < num_pages * PGSIZE);
	assert(page2pa(p1) < num_pages * PGSIZE);
	assert(page2pa(p2) < num_pages * PGSIZE);

	extern free_area_t free_area;
	struct list_head free_list_store = free_area.free_list;
	list_init_head(&free_area.free_list);
	assert(list_empty(&free_area.free_list));

	unsigned int nr_free_store = free_area.num_free;
	free_area.num_free = 0;

	assert(pm_manager->alloc_pages(1) == NULL);

	pm_manager->free_pages(p0, 1);
	pm_manager->free_pages(p1, 1);
	pm_manager->free_pages(p2, 1);
	assert(free_area.num_free == 3);

	assert((p0 = pm_manager->alloc_pages(1)) != NULL);
	assert((p1 = pm_manager->alloc_pages(1)) != NULL);
	assert((p2 = pm_manager->alloc_pages(1)) != NULL);

	assert(pm_manager->alloc_pages(1) == NULL);

	pm_manager->free_pages(p0, 1);
	assert(!list_empty(&free_area.free_list));

	struct Page *p;
	assert((p = pm_manager->alloc_pages(1)) == p0);
	assert(pm_manager->alloc_pages(1) == NULL);

	assert(free_area.num_free == 0);
	free_area.free_list = free_list_store;
	free_area.num_free = nr_free_store;

	pm_manager->free_pages(p, 1);
	pm_manager->free_pages(p1, 1);
	pm_manager->free_pages(p2, 1);
}

#endif // CORNOS_TEST_QEMU_MM_CHECK_PM_MANAGER_H
