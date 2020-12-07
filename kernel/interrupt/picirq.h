// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_KERNEL_PICIRQ_H
#define CORNOS_KERNEL_PICIRQ_H

void pic_init(void);
void pic_enable(unsigned int irq);

#define IRQ_BASE_ADDR 32 // send to 8259A ICW2

// see https://en.wikipedia.org/wiki/Interrupt_request_(PC_architecture)
#define IRQ0_TIMER 0
#define IRQ0_KEYBOARD 1

#endif //CORNOS_KERNEL_PICIRQ_H
