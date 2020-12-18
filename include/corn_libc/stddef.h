// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORN_LIBC_STDDEF_H
#define CORN_LIBC_STDDEF_H

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef _STDDEF_H
/* Return the offset of 'member' relative to the beginning of a struct type */
#define offsetof(type, member) ((size_t)(&((type *)0)->member))
#endif // _STDDEF_H

#endif // CORN_LIBC_STDDEF_H
