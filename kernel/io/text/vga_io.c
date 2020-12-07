// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "vga_io.h"
#include "cursor.h"

/* In x86 real mode, gs = 0xb800 */
static unsigned char *const VGA_BUFFER = (unsigned char *const)0xb8000;

void put_char(unsigned char ch, uint16_t position)
{
	VGA_BUFFER[position << 1] = ch;
	VGA_BUFFER[(position << 1) + 1] = 0xb;
}

void puts(unsigned char *s)
{
	uint16_t position = get_cursor_position();
	for (int i = 0; s[i]; ++i)
		put_char(s[i], position + i);
}
