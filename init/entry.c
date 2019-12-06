// kernel entry
#include "kernel.h"
#include "console.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "multiboot.h"
#include "memory.h"
#include "vmemory.h"
#include "heap.h"
#include "sched.h"

// global kernel stack top
u32int kern_stack_top;

// global mutilboot ptr
multiboot_t *glb_mboot_ptr;
// kernel stack
char kern_stack[STACK_SIZE];

// page dirctory tmp
__attribute__((section(".init.data"))) pgd_t *pgd_tmp  = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low  = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_hign = (pgd_t *)0x3000;

 int kern_logo()
{
    char* logo[5]= {                      
        "    __  __    ___        _        ___      _   ____            \n",
        "    \\ \\/ /   |_ _|      / \\      / _ \\    ( ) / ___|       \n",
        "     \\  /     | |      / _ \\    | | | |   |/  \\___ \\       \n",
        "     /  \\     | |     / ___ \\   | |_| |        ___) |        \n",
        "    /_/\\_\\   |___|   /_/   \\_\\   \\___/        |____/      \n"
    };

    console_clear(); 
    int i;
    for(i = 0; i != 5; i++)
        console_write(logo[i]);

    return 0;
}

void kern_init() {
    kern_logo();

    init_descriptor_tables();

    //asm volatile("int $0x3");
    //asm volatile("int $0x4");
    printk("\n");

    //asm volatile("sti");
    init_timer(50);

    // mm
    mem_show();
    mem_init();
    vmm_init();

    // mm alloc test
    init_heap();
    //test_heap();

    // process sched
    sched_init();
    // hlt cpu
    while (1) {
        asm volatile ("hlt");
    }
}

__attribute__((section(".init.text"))) void kern_entry() {
    pgd_tmp[0] = (u32int)pte_low | PAGE_PRESENT | PAGE_WRITE;
	pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (u32int)pte_hign | PAGE_PRESENT | PAGE_WRITE;

	// map virtual addr 4MB to first 4MB physical memory
	int i;
	for (i = 0; i < 1024; i++) {
		pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	// map physical memory 0x00000000-0x00400000 to virtual addr 0xC0000000-0xC0400000
	for (i = 0; i < 1024; i++) {
		pte_hign[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}
	
	// set temp page dirctory 
	asm volatile ("mov %0, %%cr3" : : "r" (pgd_tmp));

	u32int cr0;

	// set CPU page management
	asm volatile ("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= 0x80000000;
	asm volatile ("mov %0, %%cr0" : : "r" (cr0));
	
	// switch kernel stack
	kern_stack_top = ((u32int)kern_stack + STACK_SIZE) & 0xFFFFFFF0;
	asm volatile ("mov %0, %%esp\n\t"
			"xor %%ebp, %%ebp" : : "r" (kern_stack_top));

	// set global mutilboot ptr
	glb_mboot_ptr = mboot_ptr_tmp + PAGE_OFFSET;

    kern_init();
}
