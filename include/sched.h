#ifndef SCHED_H
#define SCHED_H

#include "task.h"

extern task_struct* running_proc_head;
extern task_struct* wait_proc_head;
extern task_struct* current;

void sched_init();
void schedule();
void change_task_to(task_struct* next);
void switch_to(context* prev, context* next);

#endif
