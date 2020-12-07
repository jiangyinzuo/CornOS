// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "idt.h"
#include "int_vectors.h"
#include "interrupt.h"
#include "../mm/layout.h"
#include "../mm/mmu.h"
#include <lib/defs.h>

#define NUM_IDT 256
static struct gate_desc_t idt[NUM_IDT] = { { 0 } };

void idt_init()
{
	extern uintptr_t __int_vectors[];
	for (int i = 0; i < NUM_IDT; ++i) {
		set_gate(idt + i, TYPE_INT_GATE32, GD_KERNEL_TEXT,
			 (uint32_t)__int_vectors[i], DPL_KERNEL);
	}
	// set for switch from user to kernel
	set_gate(idt + TRAP_SWITCH_TO_KERNEL, TYPE_INT_GATE32, GD_KERNEL_TEXT,
		 (uint32_t)__int_vectors[TRAP_SWITCH_TO_KERNEL], DPL_USER);
}
