#include "heap.h"
#include "memory.h"
#include "vmemory.h"
#include "kernel.h"

// basic chunk management
static void alloc_chunk(u32int start, u32int len);
static void free_chunk(header_t *chunk);
static void split_chunk(header_t *chunk, u32int len);
static void glue_chunk(header_t *chunk);
static u32int heap_max = HEAP_START;

// heap header
static header_t *heap_first;

void init_heap(){
    heap_first = 0;
}

void* kmalloc(u32int len) {
    // mem size must add heap header describe 
    len += sizeof(header_t);
    header_t* cur = heap_first;
    header_t* prev = 0; 

    while(cur) {
        if(cur->allocated == 0 && cur->length >= len) {
            split_chunk(cur, len);
			cur->allocated = 1;
			return (void *)((u32int)cur + sizeof(header_t));
        }

        prev = cur;
        cur = cur->next;
    }

    u32int chunk_start;
    if(prev) {
        chunk_start = (u32int)prev + prev->length;
    } else {
		chunk_start = HEAP_START;
		heap_first = (header_t *)chunk_start;
    }

	alloc_chunk(chunk_start, len);
	cur = (header_t *)chunk_start;
	cur->prev = prev;
	cur->next = 0;
	cur->allocated = 1;
	cur->length = len;
	
	if (prev) {
		prev->next = cur;
	}

	return (void*)(chunk_start + sizeof(header_t));
}

void kfree(void* p) {
    header_t* h = (header_t*)((u32int)p - sizeof(header_t));
    h->allocated = 0;

	glue_chunk(h);
}

void alloc_chunk(u32int start, u32int len) {
    while(start + len > heap_max) {
        u32int page = mem_page_alloc();
		map(pgd_kern, heap_max, page, PAGE_PRESENT | PAGE_WRITE);
		heap_max += PAGE_SIZE;
    }
}

void free_chunk(header_t* chunk) {
    if(chunk->prev == 0) {
        heap_first = 0;
    } else {
        chunk->prev->next = 0;
    }

    while((heap_max - PAGE_SIZE) >= (u32int)chunk) {
        heap_max -= PAGE_SIZE;
		u32int page;
		get_mapping(pgd_kern, heap_max, &page);
		unmap(pgd_kern, heap_max);
		mem_page_free(page);
    }
}

void split_chunk(header_t *chunk, u32int len) {
    // ought to remain heap header size
	if (chunk->length - len > sizeof (header_t)) {
		header_t *newchunk = (header_t *)((u32int)chunk + len);
		newchunk->prev = chunk;
		newchunk->next = chunk->next;
		newchunk->allocated = 0;
		newchunk->length = chunk->length - len;

		chunk->next = newchunk;
		chunk->length = len;
	}
}

void glue_chunk(header_t *chunk) {
    // glue the chunk when it has a next chunk and not be used
	if (chunk->next && chunk->next->allocated == 0) {
		chunk->length = chunk->length + chunk->next->length;
		if (chunk->next->next) {
			chunk->next->next->prev = chunk;
		}
		chunk->next = chunk->next->next;
	}

    // glue the chunk when it has a prev chunk and not be used
	if (chunk->prev && chunk->prev->allocated == 0) {
		chunk->prev->length = chunk->prev->length + chunk->length;
		chunk->prev->next = chunk->next;
		if (chunk->next) {
			chunk->next->prev = chunk->prev;
		}
		chunk = chunk->prev;
	}

	if (chunk->next == 0) {
		free_chunk(chunk);
	}
}

void test_heap(){
	printk("Test kmalloc() && kfree() now ...\n\n");

    void *addr1 = kmalloc(50);
	//printk("kmalloc    50 byte in 0x%X\n", addr1);
	//void *addr2 = kmalloc(500);
	//printk("kmalloc   500 byte in 0x%X\n", addr2);
	//void *addr3 = kmalloc(5000);
	//printk("kmalloc  5000 byte in 0x%X\n", addr3);
	//void *addr4 = kmalloc(50000);
	//printk("kmalloc 50000 byte in 0x%X\n\n", addr4);

	//printk("free mem in 0x%X\n", addr1);
	//kfree(addr1);
	//printk("free mem in 0x%X\n", addr2);
	//kfree(addr2);
	//printk("free mem in 0x%X\n", addr3);
	//kfree(addr3);
	//printk("free mem in 0x%X\n\n", addr4);
	//kfree(addr4);
}
