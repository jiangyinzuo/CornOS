// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "idt.h"

void idt_init()
{
}

void trap(struct trapframe *tf)
{
	// dispatch based on what type of trap occurred
	//	trap_dispatch(tf);
}