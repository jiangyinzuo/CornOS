// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "kernel_init.h"
#include "../interrupt/picirq.h"
#include "../interrupt/idt.h"
#include "../interrupt/interrupt.h"
#include "../device/timer8253.h"
#include "../io/text/vga.h"
#include "../mm/pmm.h"
#include "../device/keyboard.h"

void kernel_init()
{
	vga_init();

	pmm_init();
	pic_init();
	idt_init();
	kbd_init();
	timer8253_init();
	intr_enable();
}
