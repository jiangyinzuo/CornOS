// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORN_OS_LIB_DEFS_H
#define CORN_OS_LIB_DEFS_H

#include <corn_libc/stdint.h>

/* used for page numbers */
typedef size_t ppn_t;



/* *
 * to_struct - get the struct from a ptr
 * @ptr:    a struct pointer of member
 * @type:   the type of the struct this is embedded in
 * @member: the name of the member within the struct
 * */
#define to_struct(ptr, type, member) \
	((type *)((void *)(ptr)-offsetof(type, member)))

#endif /* CORN_OS_LIB_DEFS_H */
