#include "task.h"
#include "sched.h"
#include "memory.h"
#include "heap.h"
#include "common.h"
#include "kernel.h"

pid_t now_pid = 0;

u32int kthread(s32int (*fn)(void *), void* arg) {
    task_struct* t = (task_struct*)kmalloc(STACK_SIZE);
    bzero(t, sizeof(task_struct));
    
    t->state = TASK_RUNNABLE;
    t->stack = current;
    t->pid = now_pid++;
    t->mm = NULL;

    u32int* stack_top = (u32int*)((u32int)t + STACK_SIZE);
    *(--stack_top) = (u32int)arg;
    *(--stack_top) = (u32int)kthread_exit;
    *(--stack_top) = (u32int)fn;

    t->context.esp = (u32int)t + STACK_SIZE - sizeof(u32int) * 3;
	t->context.eflags = 0x200;
	t->next = running_proc_head;

	task_struct *tail = running_proc_head;
	while (tail->next != running_proc_head) {
		tail = tail->next;
	}
	tail->next = t;

	return t->pid;
}

void kthread_exit()
{
	register u32int val asm ("eax");

	printk("Thread exited with value %d\n", val);

	while (1);
}
