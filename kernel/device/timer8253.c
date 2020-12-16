// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "timer8253.h"

#include <arch/x86.h>
#include "../interrupt/picirq.h"
#include "../io/text/vga.h"

#define IO_TIMER1           0x040               // 8253 Timer #1
#define PIT_CONTROL_PORT   0x043
#define TIMER_SEL0      0x00                    // select counter 0
#define TIMER_RATEGEN   0x04                    // mode 2, rate generator
#define TIMER_16BIT     0x30                    // r/w counter 16 bits, LSB first

#define TIMER_DIV(x)    ((TIMER_FREQ + (x) / 2) / (x))

volatile size_t ticks;

void timer8253_init()
{
	outb(PIT_CONTROL_PORT, TIMER_SEL0 | TIMER_RATEGEN | TIMER_16BIT);
	outb(IO_TIMER1, TIMER_DIV(100) % 256);
	outb(IO_TIMER1, TIMER_DIV(100) / 256);

	ticks = 0;
	pic_enable(IRQ0_TIMER);
}