// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORN_OS_PRINTFMT_H
#define CORN_OS_PRINTFMT_H

#include <corn_libc/stdarg.h>
void printfmt(void (*putch)(int, void *), void *putdat, const char *fmt, ...);
void vprintfmt(void (*putch)(int, void *), void *putdat, const char *fmt,
	       va_list ap);

#endif // CORN_OS_PRINTFMT_H
