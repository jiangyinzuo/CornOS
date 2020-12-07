// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "interrupt/picirq.h"
#include "interrupt/idt.h"
#include "io/text/vga_io.h"
#include "io/text/cursor.h"

_Noreturn void corn_main()
{
	enable_cursor(0, 15);
	unsigned char m_Test[20] = "hello CornOS";
	puts(m_Test);

	pic_init();
	idt_init();
	update_cursor(3, 10);
	unsigned char m_Test1[20] = "waiting";
	puts(m_Test1);

	for (;;)
		;
}