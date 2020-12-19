// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#include "pm_manager.h"
#include <corn_libc/stdio.h>
#include "config.h"

struct PmManager *pm_manager;

extern struct PmManager first_fit, buddy, slub;

void pm_manager_init()
{
#ifndef PM_MANAGER
#define PM_MANAGER first_fit
#endif
	pm_manager = &PM_MANAGER;
	printf("use pmm manager: %s\n", pm_manager->name);
	pm_manager->init();
}