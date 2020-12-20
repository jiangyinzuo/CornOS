#ifndef LIBS_X86_H
#define LIBS_X86_H

#include <lib/defs.h>

#define do_div(n, base)                                              \
	({                                                           \
		unsigned long __upper, __low, __high, __mod, __base; \
		__base = (base);                                     \
		asm("" : "=a"(__low), "=d"(__high) : "A"(n));        \
		__upper = __high;                                    \
		if (__high != 0) {                                   \
			__upper = __high % __base;                   \
			__high = __high / __base;                    \
		}                                                    \
		asm("divl %2"                                        \
		    : "=a"(__low), "=d"(__mod)                       \
		    : "rm"(__base), "0"(__low), "1"(__upper));       \
		asm("" : "=A"(n) : "a"(__low), "d"(__high));         \
		__mod;                                               \
	})

static inline uint8_t inb(uint16_t port) __attribute__((always_inline));
static inline void insl(uint32_t port, void *addr, int cnt)
	__attribute__((always_inline));
static inline void outb(uint16_t port, uint8_t data)
	__attribute__((always_inline));
static inline void outw(uint16_t port, uint16_t data)
	__attribute__((always_inline));
static inline uint32_t read_ebp(void) __attribute__((always_inline));

/**
 * GDTR, LDTR or IDTR are 48 bits in x86 protected mode.
 * 32 bits holds the base address, 16 bits holds the offset.
 *
 * @see Intel® 64 and IA-32 Architectures Software Developer’s Manual
 * 	3A 2.4.1~3
 */
struct XDTR_t {
	uint16_t limit;
	uint32_t base_addr;
} __attribute__((packed));

static inline void lidt(struct XDTR_t *pd) __attribute__((always_inline));
static inline void sti(void) __attribute__((always_inline));
static inline void cli(void) __attribute__((always_inline));
static inline void ltr(uint16_t sel) __attribute__((always_inline));

static inline uint8_t inb(uint16_t port)
{
	uint8_t data;
	asm volatile("inb %1, %0" : "=a"(data) : "d"(port));
	return data;
}

static inline void insl(uint32_t port, void *addr, int cnt)
{
	asm volatile("cld;"
		     "repne; insl;"
		     : "=D"(addr), "=c"(cnt)
		     : "d"(port), "0"(addr), "1"(cnt)
		     : "memory", "cc");
}

/* write @data(a byte) to the @port */
static inline void outb(uint16_t port, uint8_t data)
{
	asm volatile("outb %b0, %w1" ::"a"(data), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t data)
{
	asm volatile("outw %0, %1" ::"a"(data), "d"(port));
}

static inline uint32_t read_ebp(void)
{
	uint32_t ebp;
	asm volatile("movl %%ebp, %0" : "=r"(ebp));
	return ebp;
}

static inline void lidt(struct XDTR_t *pd)
{
	asm volatile("lidt (%0)" ::"r"(pd));
}

static inline void sti(void)
{
	asm volatile("sti");
}

static inline void cli(void)
{
	asm volatile("cli");
}

static inline void ltr(uint16_t sel)
{
	asm volatile("ltr %0" ::"r"(sel));
}

static inline char *__strcpy(char *dst, const char *src)
	__attribute__((always_inline));
static inline void *__memset(void *s, char c, size_t n)
	__attribute__((always_inline));
static inline void *__memmove(void *dst, const void *src, size_t n)
	__attribute__((always_inline));
static inline void *__memcpy(void *dst, const void *src, size_t n)
	__attribute__((always_inline));

#endif /* __HAVE_ARCH_STRCMP */

#ifndef __HAVE_ARCH_STRCPY
#define __HAVE_ARCH_STRCPY
static inline char *__strcpy(char *dst, const char *src)
{
	int d0, d1, d2;
	asm volatile("1: lodsb;"
		     "stosb;"
		     "testb %%al, %%al;"
		     "jne 1b;"
		     : "=&S"(d0), "=&D"(d1), "=&a"(d2)
		     : "0"(src), "1"(dst)
		     : "memory");
	return dst;
}

#ifndef __HAVE_ARCH_MEMSET
#define __HAVE_ARCH_MEMSET
static inline void *__memset(void *s, char c, size_t n)
{
	int d0, d1;
	asm volatile("rep; stosb;"
		     : "=&c"(d0), "=&D"(d1)
		     : "0"(n), "a"(c), "1"(s)
		     : "memory");
	return s;
}
#endif /* __HAVE_ARCH_MEMSET */

#ifndef __HAVE_ARCH_MEMMOVE
#define __HAVE_ARCH_MEMMOVE
static inline void *__memmove(void *dst, const void *src, size_t n)
{
	if (dst < src) {
		return __memcpy(dst, src, n);
	}
	int d0, d1, d2;
	asm volatile("std;"
		     "rep; movsb;"
		     "cld;"
		     : "=&c"(d0), "=&S"(d1), "=&D"(d2)
		     : "0"(n), "1"(n - 1 + src), "2"(n - 1 + dst)
		     : "memory");
	return dst;
}
#endif /* __HAVE_ARCH_MEMMOVE */

#ifndef __HAVE_ARCH_MEMCPY
#define __HAVE_ARCH_MEMCPY
static inline void *__memcpy(void *dst, const void *src, size_t n)
{
	int d0, d1, d2;
	asm volatile("rep; movsl;"
		     "movl %4, %%ecx;"
		     "andl $3, %%ecx;"
		     "jz 1f;"
		     "rep; movsb;"
		     "1:"
		     : "=&c"(d0), "=&D"(d1), "=&S"(d2)
		     : "0"(n / 4), "g"(n), "1"(dst), "2"(src)
		     : "memory");
	return dst;
}
#endif /* __HAVE_ARCH_MEMCPY */

#endif /* LIBS_X86_H */
