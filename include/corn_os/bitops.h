// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORN_OS_BITOPS_H
#define CORN_OS_BITOPS_H

/**
 * test_bit - Determine whether a bit is set
 * @nr:     the bit to test
 * @addr:   the address to count from
 */
static inline _Bool test_bit(int nr, void *addr)
{
	int oldbit;
	asm volatile("btl %2, %1; sbbl %0,%0"
		     : "=r"(oldbit)
		     : "m"(*(long *)addr), "Ir"(nr));
	return oldbit != 0;
}

#endif // CORN_OS_BITOPS_H
