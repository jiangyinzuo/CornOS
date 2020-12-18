// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_KERNEL_IO_TEXT_VGA_H
#define CORNOS_KERNEL_IO_TEXT_VGA_H

#include <lib/defs.h>

#define CRT_ROWS 25
#define CRT_COLS 80

void putchar(int ch);

void vga_init();

#endif // CORNOS_KERNEL_IO_TEXT_VGA_H
