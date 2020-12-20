// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "pm_manager.h"
#include <corn_libc/assert.h>
#include <corn_libc/stddef.h>
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
	assert(free_area.num_free <= num_pages);
}

static struct Page *alloc_pages(size_t n)
{
	assert(n > 0);
	if (n > free_area.num_free) {
		return NULL;
	}
	struct Page *page = NULL;
	struct list_head *lh = &free_area.free_list;
	while ((lh = lh->next) != &free_area.free_list) {
		struct Page *p = list_head2page(lh, page_link);
		if (p->property >= n) {
			page = p;
			break;
		}
	}
	if (page != NULL) {
		if (page->property > n) {
			struct Page *p = page + n;
			p->property = page->property - n;
			set_page_property(p);
			list_add(&p->page_link, &page->page_link);
		}
		list_del(&page->page_link);
		free_area.num_free -= n;
		page->property = n;
		clear_page_property(page);
	}
	return page;
}

static void free_pages(struct Page *base, size_t n)
{
	assert(n > 0);
	struct Page *p = base;
	for (; p != base + n; ++p) {
		assert(!test_page_reserved(p));
		assert(!test_page_property(p));
		p->flags = 0;
		set_page_ref(p, 0);
	}
	base->property = n;
	set_page_property(base);
	struct list_head *lh = free_area.free_list.next;
	while (lh != &free_area.free_list) {
		p = list_head2page(lh, page_link);
		lh = lh->next;
		// TODO: optimize
		if (base + base->property == p) {
			base->property += p->property;
			clear_page_property(p);
			list_del(&(p->page_link));
		} else if (p + p->property == base) {
			p->property += base->property;
			clear_page_property(base);
			base = p;
			list_del(&(p->page_link));
		}
	}
	free_area.num_free += n;
	lh = free_area.free_list.next;
	while (lh != &free_area.free_list) {
		p = list_head2page(lh, page_link);
		if (base + base->property <= p) {
			assert(base + base->property != p);
			break;
		}
		lh = lh->next;
	}
	list_add(&(base->page_link), lh);
}

static size_t num_free_pages()
{
	return free_area.num_free;
}

EXPORT_PMM_MANAGER(first_fit)
