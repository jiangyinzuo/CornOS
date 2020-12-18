// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_KERNEL_MM_PMM_H
#define CORNOS_KERNEL_MM_PMM_H

#include "layout.h"

#define MEMMAP_ADDR 0x8000 // add KERNBASE when enabling paging

#ifndef __ASSEMBLER__

#include <corn_os/list.h>
#include <corn_os/atomic.h>

/* Flags describing the status of a page frame */
#define PG_reserved 0 // the page descriptor is reserved for kernel or unusable
#define PG_property 1 // the member 'property' is valid

#define set_page_reserved(page) atomic_set_bit(PG_reserved, &((page)->flags))
#define clear_page_reserved(page) \
	atomic_clear_bit(PG_reserved, &((page)->flags))
#define test_page_reserved(page) atomic_test_bit(PG_reserved, &((page)->flags))
#define set_page_property(page) atomic_set_bit(PG_property, &((page)->flags))
#define clear_page_property(page) \
	atomic_clear_bit(PG_property, &((page)->flags))
#define test_page_property(page)  atomic_test_bit(PG_property, &((page)->flags)

struct Page {
	int ref_cnt; // page frame's reference counter
	uint32_t flags; // array of flags that describe the status of the page frame

	// the num of free block, used in first fit pm manager
	unsigned int property;
	struct list_head page_link; // free list link
};

/* free_area_t - maintains a doubly linked list to record free (unused) pages */
typedef struct {
	struct list_head free_list; // the list header
	unsigned int nr_free; // # of free pages in this free list
} free_area_t;

void pmm_init(void);

#endif

#endif // CORNOS_KERNEL_MM_PMM_H
