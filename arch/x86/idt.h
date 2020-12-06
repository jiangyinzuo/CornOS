// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_ARCH_X86_IDT_H
#define CORNOS_ARCH_X86_IDT_H

#include <lib/defs.h>

/* registers as pushed by pushal */
struct pushregs {
	uint32_t reg_edi;
	uint32_t reg_esi;
	uint32_t reg_ebp;
	uint32_t reg_oesp; /* Useless */
	uint32_t reg_ebx;
	uint32_t reg_edx;
	uint32_t reg_ecx;
	uint32_t reg_eax;
};

struct trapframe {
	struct pushregs tf_regs;
	uint16_t tf_gs;
	uint16_t tf_padding0;
	uint16_t tf_fs;
	uint16_t tf_padding1;
	uint16_t tf_es;
	uint16_t tf_padding2;
	uint16_t tf_ds;
	uint16_t tf_padding3;
	uint32_t tf_trapno;
	/* below here defined by x86 hardware */
	uint32_t tf_err;
	uintptr_t tf_eip;
	uint16_t tf_cs;
	uint16_t tf_padding4;
	uint32_t tf_eflags;
	/* below here only when crossing rings, such as from user to kernel */
	uintptr_t tf_esp;
	uint16_t tf_ss;
	uint16_t tf_padding5;
} __attribute__((packed));

/**
 * initialize Interrupt Descriptor Table
 */
void idt_init();

void trap(struct trapframe *tf);

#endif // CORNOS_ARCH_X86_IDT_H
