#ifndef TASK_H
#define TASK_H

#include "common.h"
#include "vmemory.h"

// task state
typedef enum task_state{
    TASK_UNINIT = 0,
    TASK_SLEEPING = 1,
    TASK_RUNNABLE = 2,
    TASK_ZOMBIE = 3,
} task_state;

typedef struct context {
    u32int esp;
    u32int ebp;
    u32int ebx;
    u32int esi;
    u32int edi;
    u32int eflags;
} context;

typedef struct mm_struct {
    // task page dirctory
    pgd_t* pgd_dir;
} mm_struct;

typedef struct task_struct {
    // task state
    volatile task_state state;
    // process id
    pid_t pid;
    // kernel stack addr
    void* stack;
    // process mm addr
    mm_struct* mm;
    // process context
    context context;
    // next task ptr
    struct task_struct* next;
} task_struct;

// global process id
extern pid_t now_pid;

// kernel thread create
u32int kthread(s32int (*fn)(void *), void* arg);

void kthread_exit();

#endif
