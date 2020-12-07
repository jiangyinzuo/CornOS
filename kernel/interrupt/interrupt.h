// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_ARCH_X86_INTERRUPT_H
#define CORNOS_ARCH_X86_INTERRUPT_H

/**
 * These are arbitrarily chosen, but with care not to overlap
 * processor defined exceptions or interrupt vectors.
 */
#define TRAP_SWITCH_TO_USER 120 // user/kernel switch
#define TRAP_SWITCH_TO_KERNEL 121 // user/kernel switch

#endif // CORNOS_ARCH_X86_INTERRUPT_H
