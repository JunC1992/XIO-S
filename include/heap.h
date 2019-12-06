#ifndef HEAP_H
#define HEAP_H

#include "common.h"

// heap start virtual address
#define HEAP_START 0xE0000000

// heap struct
typedef struct header {
    struct header* prev;
    struct header* next;
    u32int allocated : 1;
    u32int length : 31;
} header_t;

void heap_init();
void* kmalloc(u32int len);
void kfree();
void test_heap();

#endif
