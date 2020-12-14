// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_ARCH_X86_INTERRUPT_H
#define CORNOS_ARCH_X86_INTERRUPT_H

/**
 * These are arbitrarily chosen, but with care not to overlap
 * processor defined exceptions or interrupt vectors.
 */
#define TRAP_SWITCH_TO_USER 120 // user/kernel switch
#define TRAP_SWITCH_TO_KERNEL 121 // user/kernel switch

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

struct trap_frame {
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

void intr_enable(void);
void intr_disable(void);

void trap(struct trap_frame *tf);

#endif // CORNOS_ARCH_X86_INTERRUPT_H
