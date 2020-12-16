// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "vga.h"
#include "cursor.h"
#include <lib/defs.h>

/* In x86 real mode, gs = 0xb800 */
static uint16_t *const VGA_BUFFER = (uint16_t *const)0xb8000;
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

/**
 * puts - writes the string pointed by @s to stdout and
 * appends a newline character.
 */
int puts(char *s)
{
	int i;
	for (i = 0; s[i]; ++i)
		putchar(s[i]);
	putchar('\n');
	return i + 1;
}
