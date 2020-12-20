// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <corn_libc/stdio.h>
#include <corn_libc/assert.h>
#include <corn_libc/string.h>
#include <kernel/io/text/vga.h>
#include <kernel/mm/pmm.h>
#include <kernel/mm/pm_manager/pm_manager.h>
#include "check_pm_manager.h"

static void first_fit_check(void)
{
	int count = 0, total = 0;
	extern free_area_t free_area;
	struct list_head *lh = &free_area.free_list;
	while ((lh = lh->next) != &free_area.free_list) {
		struct Page *p = list_head2page(lh, page_link);
		assert(test_page_property(p));
		++count, total += p->property;
	}
	assert(total == pm_manager->num_free_pages());

	basic_check();

	struct Page *p0 = pm_manager->alloc_pages(5), *p1, *p2;
	assert(p0 != NULL);
	assert(!test_page_property(p0));

	struct list_head free_list_store = free_area.free_list;
	list_init_head(&free_area.free_list);
	assert(list_empty(&free_area.free_list));
	assert(pm_manager->alloc_pages(1) == NULL);

	unsigned int nr_free_store = free_area.num_free;
	free_area.num_free = 0;

	pm_manager->free_pages(p0 + 2, 3);
	assert(pm_manager->alloc_pages(4) == NULL);
	assert(test_page_property(p0 + 2) && p0[2].property == 3);
	assert((p1 = pm_manager->alloc_pages(3)) != NULL);
	assert(pm_manager->alloc_pages(1) == NULL);
	assert(p0 + 2 == p1);

	p2 = p0 + 1;
	pm_manager->free_pages(p0, 1);
	pm_manager->free_pages(p1, 3);
	assert(test_page_property(p0) && p0->property == 1);
	assert(test_page_property(p1) && p1->property == 3);

	assert((p0 = pm_manager->alloc_pages(1)) == p2 - 1);
	pm_manager->free_pages(p0, 1);
	assert((p0 = pm_manager->alloc_pages(2)) == p2 + 1);

	pm_manager->free_pages(p0, 2);
	pm_manager->free_pages(p2, 1);

	assert((p0 = pm_manager->alloc_pages(5)) != NULL);
	assert(pm_manager->alloc_pages(1) == NULL);

	assert(free_area.num_free == 0);
	free_area.num_free = nr_free_store;

	free_area.free_list = free_list_store;
	pm_manager->free_pages(p0, 5);

	lh = &free_area.free_list;
	while ((lh = lh->next) != &free_area.free_list) {
		struct Page *p = list_head2page(lh, page_link);
		count--, total -= p->property;
	}
	assert(count == 0);
	assert(total == 0);
}

_Noreturn void corn_main()
{
	puts("first_fit_test");
	vga_init();
	pmm_init();
	puts("===== check =====");
	first_fit_check();

	assert(strcmp(pm_manager->name, "first_fit") == 0);
	printf("%u\n", pm_manager->num_free_pages());
	size_t num_free_begin = pm_manager->num_free_pages();
	assert(num_free_begin < num_pages);
	printf("%u\n", num_pages);
	size_t n = 10;
	struct Page *p = pm_manager->alloc_pages(n);
	assert(p->property == n);
	assert(pm_manager->num_free_pages() + n == num_free_begin);
	for (;;) {
	}
}
