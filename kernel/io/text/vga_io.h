// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_KERNEL_IO_TEXT_VGA_IO_H
#define CORNOS_KERNEL_IO_TEXT_VGA_IO_H

#include <lib/defs.h>

void put_char(unsigned char ch, uint16_t position);
void puts(unsigned char *s);

#endif // CORNOS_KERNEL_IO_TEXT_VGA_IO_H
