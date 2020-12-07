// Copyright (c) 2020, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_KERNEL_PICIRQ_H
#define CORNOS_KERNEL_PICIRQ_H

void pic_init(void);
void pic_enable(unsigned int irq);

#define IRQ_OFFSET 32

#endif //CORNOS_KERNEL_PICIRQ_H
