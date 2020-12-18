// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORN_OS_ALGORITHM_H
#define CORN_OS_ALGORITHM_H

#define MAX(a, b)                      \
	({                             \
		__typeof__(a) _a_ = a; \
		__typeof__(b) _b_ = b; \
		_a_ > _b_ ? _a_ : _b_; \
	})

#define MIN(a, b)                      \
	({                             \
		__typeof__(a) _a_ = a; \
		__typeof__(b) _b_ = b; \
		_a_ < _b_ ? _a_ : _b_; \
	})

/**
 * Rounding operations (efficient when @n is a power of 2)
 * Round down to the nearest multiple of @n
 */
#define ROUNDDOWN(a, n)                       \
	({                                    \
		size_t __a = (size_t)(a);     \
		(typeof(a))(__a - __a % (n)); \
	})

/* Round up to the nearest multiple of n */
#define ROUNDUP(a, n)                                               \
	({                                                          \
		size_t __n = (size_t)(n);                           \
		(typeof(a))(ROUNDDOWN((size_t)(a) + __n - 1, __n)); \
	})

#endif // CORN_OS_ALGORITHM_H
