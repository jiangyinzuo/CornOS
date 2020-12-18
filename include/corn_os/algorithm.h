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

#endif // CORN_OS_ALGORITHM_H
