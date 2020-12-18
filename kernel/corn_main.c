// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include <corn_libc/stdio.h>
#include "interrupt/picirq.h"
#include "interrupt/idt.h"
#include "interrupt/interrupt.h"
#include "device/timer8253.h"
#include "io/text/vga.h"
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
	kernel_init();
	char m_Test[20] = "hello CornOS";
	puts(m_Test);

	char m_Test1[20] = "waiting";
	puts(m_Test1);
	printf("hello %d\n", 123);
	for (;;)
		;
}