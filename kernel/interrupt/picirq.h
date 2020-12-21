// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_KERNEL_PICIRQ_H
#define CORNOS_KERNEL_PICIRQ_H

#include <corn_libc/stdint.h>

#define IRQ_BASE_ADDR 32 // send to 8259A ICW2, 0 - 31 is used for exception/NMI

// see https://en.wikipedia.org/wiki/Interrupt_request_(PC_architecture)
#define IRQ0_TIMER 0
#define IRQ1_KEYBOARD 1

extern uint16_t irq_mask_8259A;

void pic_init(void);
void pic_enable(unsigned int irq);
void pic_set_mask_8259A(uint16_t mask);

#endif //CORNOS_KERNEL_PICIRQ_H
