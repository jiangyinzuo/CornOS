// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORN_OS_ATOMIC_H
#define CORN_OS_ATOMIC_H

static inline void __attribute__((always_inline))
atomic_set_bit(long nr, volatile void *addr)
{
	asm volatile("lock btsl %1, %0"
		     : "=m"(*(volatile long *)addr)
		     : "Ir"(nr));
}
#endif // CORN_OS_ATOMIC_H
