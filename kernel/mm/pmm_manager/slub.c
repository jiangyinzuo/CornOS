// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "pmm_manager.h"

static void init()
{
}

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

EXPORT_PMM_MANAGER(slub)
