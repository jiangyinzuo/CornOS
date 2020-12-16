// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_LIBC_STDINT_H
#define CORNOS_LIBC_STDINT_H

#if (!defined(_STDIO_H))

#define __always_inline inline __attribute__((always_inline))
#define __noinline __attribute__((noinline))
#define __noreturn __attribute__((noreturn))

/* Explicitly-sized versions of integer types */
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

/*
 * Pointers and addresses are 32 bits long.
 * We use pointer types to represent addresses,
 * uintptr_t to represent the numerical values of addresses.
 */
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

/* size_t is used for memory object sizes */
typedef uintptr_t size_t;
#endif

#endif // CORNOS_LIBC_STDINT_H
