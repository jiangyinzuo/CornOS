#ifndef ARCH_X86_MM_LAYOUT_H
#define ARCH_X86_MM_LAYOUT_H

/* This file contains the definitions for memory management in our OS. */

/* global segment number */
#define SEG_KERNEL_TEXT 1
#define SEG_KERNEL_DATA 2
#define SEG_USER_TEXT 3
#define SEG_USER_DATA 4
#define SEG_TSS 5

/**
 * Global Descriptor Numbers.
 *
 * Segment selector is 16 bit-length.
 * The low 0-1 bits store RPL, which has 0~3 privileges.
 * The 2 bit is Table Indicator, represents whether the selector
 * is in GDT or LDT.
 * The 3-15 bit is the index of segment descriptor. So it should
 * be left shifted by 3.
 */
#define GD_KERNEL_TEXT ((SEG_KERNEL_TEXT) << 3) // kernel text
#define GD_KERNEL_DATA ((SEG_KERNEL_DATA) << 3) // kernel data
#define GD_USER_TEXT ((SEG_USER_TEXT) << 3) // user text
#define GD_USER_DATA ((SEG_USER_DATA) << 3) // user data
#define GD_TSS ((SEG_TSS) << 3) // task segment selector

#define DPL_KERNEL (0)
#define DPL_USER (3)

#define KERNEL_CS ((GD_KERNEL_TEXT) | DPL_KERNEL)
#define KERNEL_DS ((GD_KERNEL_DATA) | DPL_KERNEL)
#define USER_CS ((GD_USER_TEXT) | DPL_USER)
#define USER_DS ((GD_USER_DATA) | DPL_USER)

/*
 * Virtual memory map:                                          Permissions
 *                                                              kernel/user
 *
 *     4G ------------------> +---------------------------------+
 *                            |                                 |
 *                            |         Empty Memory (*)        |
 *                            |                                 |
 *                            +---------------------------------+ 0xFB000000
 *                            |   Cur. Page Table (Kern, RW)    | RW/-- PTSIZE
 *     VPT -----------------> +---------------------------------+ 0xFAC00000
 *                            |        Invalid Memory (*)       | --/--
 *     KERNTOP -------------> +---------------------------------+ 0xF8000000
 *                            |                                 |
 *                            |    Remapped Physical Memory     | RW/-- KMEMSIZE
 *                            |                                 |
 *     KERNBASE ------------> +---------------------------------+ 0xC0000000
 *                            |                                 |
 *                            |                                 |
 *                            |                                 |
 *                            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * (*) Note: The kernel ensures that "Invalid Memory" is *never* mapped.
 *     "Empty Memory" is normally unmapped, but user programs may map pages
 *     there if desired.
 *
 * 0G ~ 3G is user space; 3G ~ 4G is kernel space
 */

/* All physical memory mapped at this address */
#define KERNBASE 0xC0000000
#define KMEMSIZE 0x38000000 // the maximum amount of physical memory
#define KERNTOP (KERNBASE + KMEMSIZE)

/* *
 * Virtual page table. Entry PDX[VPT] in the PD (Page Directory) contains
 * a pointer to the page directory itself, thereby turning the PD into a page
 * table, which maps all the PTEs (Page Table Entry) containing the page mappings
 * for the entire virtual address space into that 4 Meg region starting at VPT.
 * */
#define VPT 0xFAC00000

#define KSTACKPAGE 2 // # of pages in kernel stack
#define KSTACKSIZE (KSTACKPAGE * PGSIZE) // sizeof kernel stack

#ifndef __ASSEMBLER__

#include <corn_libc/stdint.h>

// some constants for bios interrupt 15h AX = 0xE820
#define E820MAX 20 // number of entries in E820MAP
#define E820_ADDR_RANGE_MEM 1 // address range memory, os can use this memory
#define E820_ADDR_RANGE_RESERVED 2 // address range reserved, os can not use

struct e820map {
	int nr_map;

	// address range descriptor structure
	struct {
		uint64_t addr;
		uint64_t size;
		uint32_t type;
	} __attribute__((packed)) map[E820MAX];
};

static inline uintptr_t __attribute__((always_inline))
kern_physical_addr(uintptr_t virtual_addr)
{
	return virtual_addr - KERNBASE;
}

static inline uintptr_t __attribute__((always_inline))
kern_virtual_addr(uintptr_t physical_addr)
{
	return physical_addr + KERNBASE;
}

#endif // __ASSEMBLER__

#endif // ARCH_X86_MM_LAYOUT_H
