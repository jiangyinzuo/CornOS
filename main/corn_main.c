// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include <corn_libc/stdio.h>
#include "kernel/init/kernel_init.h"
#include "kernel/mm/pmm.h"

_Noreturn void corn_main()
{
	char m_Test[20] = "hello CornOS";
	puts(m_Test);

	kernel_init();

	char m_Test1[20] = "waiting";
	puts(m_Test1);
	print_page_table();
	puts("input a character:");
	char ch = getchar();
	printf("your input: %c\n", ch);
	for (;;) {
	}
}