// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "interrupt/picirq.h"
#include "interrupt/idt.h"
#include "interrupt/interrupt.h"
#include "device/timer8253.h"
#include "io/text/vga.h"
#include "io/text/cursor.h"
#include "mm/pmm.h"

void kernel_init()
{
	vga_init();
	pmm_init();
	pic_init();
	idt_init();
	timer8253_init();
	intr_enable();
}

_Noreturn void corn_main()
{
	enable_cursor(0, 15);
	char m_Test[20] = "hello CornOS";
	puts(m_Test);

	update_cursor_xy(0, 9);
	kernel_init();

	update_cursor_xy(3, 10);
	char m_Test1[20] = "waiting";
	puts(m_Test1);

	for (;;)
		;
}