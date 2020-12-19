// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "pm_manager.h"

free_area_t free_area;

static void init()
{
	list_init_head(&free_area.free_list);
	free_area.num_free = 0;
}

/**
 * initialize memories
 * @param base: base address of these pages
 * @param n: number of pages
 */
static void init_memmap(struct Page *base, size_t n)
{
}

static struct Page *alloc_pages(size_t n)
{
	return 0;
}

static void free_pages(struct Page *base, size_t n)
{
}

static size_t num_free_pages()
{
	return 0;
}

EXPORT_PMM_MANAGER(first_fit)
