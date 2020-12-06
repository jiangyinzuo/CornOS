// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

void putchar(unsigned char ch, int i)
{
	// 实模式下gs=0xb800
	static unsigned char *vga_buffer = (unsigned char *)0xb8000;
	vga_buffer[i << 1] = ch;
	vga_buffer[(i << 1) + 1] = 0xb;
}

void puts(unsigned char *s)
{
	for (int i = 0; s[i]; ++i)
		putchar(s[i], i);
}

_Noreturn void corn_main()
{
	unsigned char m_Test[20] = "hello CornOS";
	puts(m_Test);
	for (;;) {
	}
}