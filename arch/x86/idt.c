// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "idt.h"
#include "int_vectors.h"
#include "mm/mmu.h"

#define NUM_IDT 256
static struct gate_desc_t idt[NUM_IDT] = { { 0 } };

void idt_init()
{
	extern uintptr_t __int_vectors[];
	for (int i = 0; i < NUM_IDT; ++i) {
//		set_gate(idt + i, TYPE_INT_GATE32, )
//		SETGATE(idt[i], 0, GD_KTEXT, __vectors[i], DPL_KERNEL);
	}
	// set for switch from user to kernel
//	SETGATE(idt[T_SWITCH_TOK], 0, GD_KTEXT, __vectors[T_SWITCH_TOK],
//		DPL_USER)
}

void trap(struct trapframe *tf)
{
	// dispatch based on what type of trap occurred
	//	trap_dispatch(tf);
}