#include "sched.h"
#include "memory.h"
#include "heap.h"

task_struct* running_proc_head = NULL;
task_struct* wait_proc_head = NULL;
task_struct* current = NULL;

void sched_init(){
    current = (task_struct*)(kern_stack_top - STACK_SIZE);

    current->state = TASK_RUNNABLE;
    current->pid = now_pid++;
    current->stack = current;
    current->mm = NULL;
    current->next = current;
    running_proc_head = current;
}

void schedule() {
    if(current) {
        change_task_to(current->next);
    }
}

void change_task_to(task_struct* next){
    if(current != next) {
        task_struct* prev = current;
        current = next;
        switch_to(&(prev->context), &(next->context));
    }
}
