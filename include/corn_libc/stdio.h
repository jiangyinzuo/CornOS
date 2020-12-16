#ifndef __LIBS_STDIO_H__
#define __LIBS_STDIO_H__

#include "../../lib/defs.h"
#include "stdarg.h"

/* kern/libs/stdio.c */
int cprintf(const char *fmt, ...);
int vcprintf(const char *fmt, va_list ap);
void cputchar(int c);
int cputs(const char *str);
int getchar(void);

/* kern/libs/readline.c */
char *readline(const char *prompt);

/* libs/printfmt.c */
int snprintf(char *str, size_t size, const char *fmt, ...);
int vsnprintf(char *str, size_t size, const char *fmt, va_list ap);

#endif /* !__LIBS_STDIO_H__ */
