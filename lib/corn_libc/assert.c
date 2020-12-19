// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef NDEBUG

#include <corn_libc/assert.h>
#include <corn_libc/stdio.h>

_Noreturn void __assert_fail(const char *expr, const char *file, int line)
{
	printf("assert failed in %s: line %d\n", file, line);
	printf("failed: %s", expr);
	for (;;)
		;
}
#endif
