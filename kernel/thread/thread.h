// Copyright (c) 2021, Jiang Yinzuo. All rights reserved.

#ifndef CORNOS_KERNEL_THREAD_THREAD_H
#define CORNOS_KERNEL_THREAD_THREAD_H

/**
 * Status of process or thread
 */
enum task_status {
	TASK_RUNNING,
	TASK_READY,
	TASK_BLOCKED,
	TASK_WAITING,
	TASK_HANGING,
	TASK_DIED
};

#endif // CORNOS_KERNEL_THREAD_THREAD_H
