// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORN_LIBC_ASSERT_H
#define CORN_LIBC_ASSERT_H

#ifdef NDEBUG
#define assert(expr) ((void)(0))
#else

_Noreturn void __assert_fail(const char *expr, const char *file, int line);

#define assert(expr) \
	(expr) ? ((void)(0)) : __assert_fail(#expr, __FILE__, __LINE__)
#endif
#endif // CORNOS_INCLUDE_CORN_LIBC_ASSERT_H
