// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "pm_manager.h"
#include <corn_libc/assert.h>

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
	assert(n > 0);
	for (struct Page *p = base; p < base + n; ++p) {
		assert(test_page_reserved(p));
		p->flags = 0;
		p->property = 0;
		p->ref_cnt = 0;
	}
	base->property = n;
	set_page_property(base);
	free_area.num_free += n;
	list_add(&base->page_link, &free_area.free_list);
}

static struct Page *alloc_pages(size_t n)
{
	assert(n > 0);
	return 0;
}

static void free_pages(struct Page *base, size_t n)
{
	assert(n > 0);
}

static size_t num_free_pages()
{
	return free_area.num_free;
}

EXPORT_PMM_MANAGER(first_fit)
