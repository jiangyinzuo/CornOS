// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "vga.h"
#include "cursor.h"
#include <lib/defs.h>
#include <kernel/mm/layout.h>

/*
 * In x86 real mode, gs = 0xb800
 * When enabling paging, it should add KERNBASE
 */
static uint16_t *const VGA_BUFFER = (uint16_t *const)(0xb8000 + KERNBASE);
static uint16_t vga_pos;

void vga_init()
{
	update_cursor(0);
}

/**
 * putchar - writes a single character @ch to stdout
 */
void putchar(int ch)
{
	// set black on white
	if (!(ch & ~0xFF)) {
		ch |= 0x0700;
	}

	switch (ch & 0xff) {
	case '\b':
		if (vga_pos > 0) {
			--vga_pos;
			VGA_BUFFER[vga_pos] = (ch & ~0xff) | ' ';
		}
		break;
	case '\n':
		vga_pos += CRT_COLS;
	case '\r':
		vga_pos -= (vga_pos % CRT_COLS);
		break;
	default:
		VGA_BUFFER[vga_pos++] = ch; // write the character
		break;
	}
	update_cursor(vga_pos);
}

