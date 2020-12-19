// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include <corn_libc/stdio.h>
#include "kernel/init/kernel_init.h"

_Noreturn void corn_main()
{
	char m_Test[20] = "hello CornOS";
	puts(m_Test);

	kernel_init();

	char m_Test1[20] = "waiting";
	puts(m_Test1);
	printf("hello %d\n", 123);
	for (;;) {
	}
}