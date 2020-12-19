// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_KERNEL_MM_PAGE_H
#define CORNOS_KERNEL_MM_PAGE_H

/*
 * A linear address 'la' has a three-part structure as follows:
 *
 * +--------10------+-------10-------+---------12----------+
 * | Page Directory |   Page Table   | Offset within Page  |
 * |      Index     |     Index      |                     |
 * +----------------+----------------+---------------------+
 *  \--- PDX(la) --/ \--- PTX(la) --/ \---- PGOFF(la) ----/
 *  \----------- PPN(la) -----------/
 *
 * The PDX, PTX, PGOFF, and PPN macros decompose linear addresses as shown.
 * To construct a linear address la from PDX(la), PTX(la), and PGOFF(la),
 * use PGADDR(PDX(la), PTX(la), PGOFF(la)).
 */

// page directory index
#define PDX(la) ((((uintptr_t)(la)) >> PDXSHIFT) & 0x3FF)

// page table index
#define PTX(la) ((((uintptr_t)(la)) >> PTXSHIFT) & 0x3FF)

// page number field of address
#define PPN(la) (((uintptr_t)(la)) >> PTXSHIFT)

// offset in page
#define PGOFF(la) (((uintptr_t)(la)) & 0xFFF)

// construct linear address from indexes and offset
#define PGADDR(d, t, o) ((uintptr_t)((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

// address in page table or page directory entry
#define PTE_ADDR(pte) ((uintptr_t)(pte) & ~0xFFF)
#define PDE_ADDR(pde) PTE_ADDR(pde)

/* page directory and page table constants */
#define NPDEENTRY 1024 // page directory entries per page directory
#define NPTEENTRY 1024 // page table entries per page table

#define PGSIZE 4096 // bytes mapped by a page (4kb)
#define PGSHIFT 12 // log2(PGSIZE)
#define PTSIZE (PGSIZE * NPTEENTRY) // bytes mapped by a page directory entry
#define PTSHIFT 22 // log2(PTSIZE)

#define PTXSHIFT 12 // offset of PTX in a linear address
#define PDXSHIFT 22 // offset of PDX in a linear address

/* page table/directory entry flags */
#define PTE_P 0x001 // Present
#define PTE_W 0x002 // Writeable
#define PTE_U 0x004 // User
#define PTE_PWT 0x008 // Write-Through
#define PTE_PCD 0x010 // Cache-Disable
#define PTE_A 0x020 // Accessed
#define PTE_D 0x040 // Dirty
#define PTE_PS 0x080 // Page Size
#define PTE_MBZ 0x180 // Bits must be zero
#define PTE_AVAIL 0xE00 // Available for software use
// The PTE_AVAIL bits aren't used by the kernel or interpreted by the
// hardware, so user processes are allowed to set them arbitrarily.

#define PTE_USER (PTE_U | PTE_W | PTE_P)

/* Flags describing the status of a page frame */
#define PG_reserved 0 // the page descriptor is reserved for kernel or unusable
#define PG_property 1 // the member 'property' is valid

#ifndef __ASSEMBLER__

#include <corn_libc/stdint.h>
#include <corn_os/list.h>
#include <corn_os/bitops.h>
#include <corn_os/atomic.h>
#include "layout.h"

typedef uintptr_t pte_t; // page table entry
typedef uintptr_t pde_t; // page directory entry

struct Page {
	int ref_cnt; // page frame's reference counter
	uint32_t flags; // array of flags that describe the status of the page frame

	// the num of free block, used in first fit pm manager
	unsigned int property;
	struct list_head page_link; // free list link
};

extern struct Page *pages;
extern size_t num_pages;

static inline ppn_t page2ppn(struct Page *page)
{
	return page - pages;
}

static inline uintptr_t page2pa(struct Page *page)
{
	return page2ppn(page) << PGSHIFT;
}

static inline struct Page *pa2page(uintptr_t pa)
{
	return &pages[PPN(pa)];
}

static inline void *page2kva(struct Page *page)
{
	return (void *)kern_physical_addr(page2pa(page));
}

static inline struct Page *kva2page(uintptr_t kva)
{
	return pa2page(kern_physical_addr(kva));
}

static inline struct Page *pte2page(pte_t pte)
{
	return pa2page(PTE_ADDR(pte));
}

static inline struct Page *pde2page(pde_t pde)
{
	return pa2page(PDE_ADDR(pde));
}

static inline int page_ref(struct Page *page)
{
	return page->ref_cnt;
}

static inline void set_page_ref(struct Page *page, int val)
{
	page->ref_cnt = val;
}

static inline int page_ref_inc(struct Page *page)
{
	page->ref_cnt += 1;
	return page->ref_cnt;
}

static inline int page_ref_dec(struct Page *page)
{
	page->ref_cnt -= 1;
	return page->ref_cnt;
}

static inline void set_page_reserved(struct Page *page)
{
	atomic_set_bit(PG_reserved, &(page->flags));
}
#define clear_page_reserved(page) \
	atomic_clear_bit(PG_reserved, &((page)->flags))

static inline _Bool test_page_reserved(struct Page *page)
{
	return test_bit(PG_reserved, &(page->flags));
}

static inline void set_page_property(struct Page *page)
{
	atomic_set_bit(PG_property, &(page->flags));
}
#define clear_page_property(page) \
	atomic_clear_bit(PG_property, &((page)->flags))
#define test_page_property(page)  test_bit(PG_property, &((page)->flags)

void page_init();

#endif // __ASSEMBLER__

#endif // CORNOS_KERNEL_MM_PAGE_H
