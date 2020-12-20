// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_ARCH_X86_STRING_H
#define CORNOS_ARCH_X86_STRING_H

static inline int __attribute__((always_inline))
__strcmp(const char *s1, const char *s2)
{
	int d0, d1, ret;
	asm volatile("1: lodsb;"
		     "scasb;"
		     "jne 2f;"
		     "testb %%al, %%al;"
		     "jne 1b;"
		     "xorl %%eax, %%eax;"
		     "jmp 3f;"
		     "2: sbbl %%eax, %%eax;"
		     "orb $1, %%al;"
		     "3:"
		     : "=a"(ret), "=&S"(d0), "=&D"(d1)
		     : "1"(s1), "2"(s2)
		     : "memory");
	return ret;
}

static inline void *__attribute__((always_inline))
__memset(void *s, char c, size_t n)
{
	int d0, d1;
	asm volatile("rep; stosb;"
		     : "=&c"(d0), "=&D"(d1)
		     : "0"(n), "a"(c), "1"(s)
		     : "memory");
	return s;
}

#endif // CORNOS_ARCH_X86_STRING_H
