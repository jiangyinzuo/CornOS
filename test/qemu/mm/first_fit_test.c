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

_Noreturn void corn_main()
{
	puts("first_fit_test");
	vga_init();
	pmm_init();
	puts("===== basic check =====");
	basic_check();

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
