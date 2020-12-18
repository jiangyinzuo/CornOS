// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include <lib/defs.h>
#include <corn_libc/stdio.h>

/* HIGH level console I/O */

/* *
 * putch - writes a single character @c to stdout, and it will
 * increace the value of counter pointed by @cnt.
 * */
static void putch(int c, int *cnt)
{
	putchar(c);
	(*cnt)++;
}

extern void vprintfmt(void (*putch)(int, void *), void *putdat, const char *fmt,
		      va_list ap);

/* *
 * vprintf - format a string and writes it to stdout
 *
 * The return value is the number of characters which would be
 * written to stdout.
 *
 * Call this function if you are already dealing with a va_list.
 * Or you probably want cprintf() instead.
 * */
int vprintf(const char *fmt, va_list ap)
{
	int cnt = 0;
	vprintfmt((void *)putch, &cnt, fmt, ap);
	return cnt;
}

/* *
 * printf - formats a string and writes it to stdout
 *
 * The return value is the number of characters which would be
 * written to stdout.
 * */
int printf(const char *fmt, ...)
{
	va_list ap;
	int cnt;
	va_start(ap, fmt);
	cnt = vprintf(fmt, ap);
	va_end(ap);
	return cnt;
}

/**
 * puts - writes the string pointed by @s to stdout and
 * appends a newline character.
 */
int puts(const char *s)
{
	int i = 0;
	for (; s[i]; ++i) {
		putchar(s[i]);
	}
	putchar('\n');
	return i + 1;
}

///* getchar - reads a single non-zero character from stdin */
//int getchar(void)
//{
//	int c;
//	while ((c = cons_getc()) == 0)
//		/* do nothing */;
//	return c;
//}
