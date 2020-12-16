// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_LIBC_STDARG_H
#define CORNOS_LIBC_STDARG_H

/* compiler provides size of save area */
typedef __builtin_va_list va_list;

#define va_start(ap, last) (__builtin_va_start(ap, last))
#define va_arg(ap, type) (__builtin_va_arg(ap, type))
#define va_end(ap) /*nothing*/

#endif /* CORNOS_LIBC_STDARG_H */
