// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "interrupt.h"
#include "picirq.h"

void trap(struct trap_frame *tf)
{
	switch (tf->tf_trapno) {
	case IRQ_BASE_ADDR + IRQ0_TIMER:
		break;
	}
}
