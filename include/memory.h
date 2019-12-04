#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

// physical memory max size 512M
#define PMM_MAX_SIZE 0x20000000
// physical memory page size 4k
#define PMM_PAGE_SIZE 0x1000
#define PAGE_MAX_SIZE (PMM_MAX_SIZE/PMM_PAGE_SIZE) 

#define STACK_SIZE 8192

extern u8int kern_start[];
extern u8int kern_end[];

void mem_show();
void mem_init();

u32int mem_page_alloc();
void mem_page_free(u32int addr);

#endif
