// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

// see https://en.wikipedia.org/wiki/Intel_8253

#ifndef CORNOS_KERNEL_DEVICE_TIMER_8253_H
#define CORNOS_KERNEL_DEVICE_TIMER_8253_H

#include <lib/defs.h>

// 105/88
#define TIMER_FREQ 1193182

extern volatile size_t ticks;

void timer8253_init();

#endif // CORNOS_KERNEL_DEVICE_TIMER_8253_H
