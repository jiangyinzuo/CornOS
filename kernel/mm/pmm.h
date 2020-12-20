// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_KERNEL_MM_PMM_H
#define CORNOS_KERNEL_MM_PMM_H

#include "layout.h"

#define MEMMAP_ADDR 0x8000 // add KERNBASE when enabling paging

#ifndef __ASSEMBLER__

#include <corn_os/list.h>
#include <corn_os/atomic.h>

extern char bootstack[], bootstacktop[];

void pmm_init(void);

#endif

#endif // CORNOS_KERNEL_MM_PMM_H
