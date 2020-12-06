// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_ARCH_X86_INT_VECTORS_H
#define CORNOS_ARCH_X86_INT_VECTORS_H

#define BUILD_1VECTORS(n) \
	.globl vector##n; \
	vector##n:;       \
	pushl $0;         \
	pushl $##n;       \
	jmp __alltraps;

#define WRITE_1VECTORS(n) .long vector##n;

#define DO_16VECTORS(op, h) \
	op##_1VECTORS(0x##h##0)   \
	op##_1VECTORS(0x##h##1)   \
	op##_1VECTORS(0x##h##2)   \
	op##_1VECTORS(0x##h##3)   \
	op##_1VECTORS(0x##h##4)   \
	op##_1VECTORS(0x##h##5)   \
	op##_1VECTORS(0x##h##6)   \
	op##_1VECTORS(0x##h##7)   \
	op##_1VECTORS(0x##h##8)   \
	op##_1VECTORS(0x##h##9)   \
	op##_1VECTORS(0x##h##a)   \
	op##_1VECTORS(0x##h##b)   \
	op##_1VECTORS(0x##h##c)   \
	op##_1VECTORS(0x##h##d)   \
	op##_1VECTORS(0x##h##e)   \
	op##_1VECTORS(0x##h##f)

#define BUILD_16VECTORS(h) DO_16VECTORS(BUILD, h)
#define WRITE_16VECTORS(h) DO_16VECTORS(WRITE, h)

#define DO_256VECTORS(op)   \
	DO_16VECTORS(op, 0) \
	DO_16VECTORS(op, 1) \
	DO_16VECTORS(op, 2) \
	DO_16VECTORS(op, 3) \
	DO_16VECTORS(op, 4) \
	DO_16VECTORS(op, 5) \
	DO_16VECTORS(op, 6) \
	DO_16VECTORS(op, 7) \
	DO_16VECTORS(op, 8) \
	DO_16VECTORS(op, 9) \
	DO_16VECTORS(op, a) \
	DO_16VECTORS(op, b) \
	DO_16VECTORS(op, c) \
	DO_16VECTORS(op, d) \
	DO_16VECTORS(op, e) \
	DO_16VECTORS(op, f)

#define BUILD_256VECTORS DO_256VECTORS(BUILD)
#define WRITE_256VECTORS DO_256VECTORS(WRITE)

#endif // CORNOS_ARCH_X86_INT_VECTORS_H
