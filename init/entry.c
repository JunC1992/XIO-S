// kernel entry
#include "kernel.h"
#include "console.h"
#include "descriptor_tables.h"
#include "timer.h"

extern u8int kern_start[];
extern u8int kern_end[];

int kern_logo()
{
    char* logo[5]= {                      
        "    __  __    ___        _        ___           \n",
        "    \\ \\/ /   |_ _|      / \\      / _ \\      \n",
        "     \\  /     | |      / _ \\    | | | |       \n",
        "     /  \\     | |     / ___ \\   | |_| |       \n",
        "    /_/\\_\\   |___|   /_/   \\_\\   \\___/     \n"
    };

    console_clear(); 
    int i;
    for(i = 0; i != 5; i++)
        console_write(logo[i]);

    return 0;
}

void kern_entry() {
    kern_logo();

    init_descriptor_tables();

    asm volatile("int $0x3");
    asm volatile("int $0x4");

    //asm volatile("sti");
    init_timer(50);
    printk("kernel start at:\t0x%08X\n", kern_start);
    printk("kernel end at:\t\t0x%08X\n", kern_end);
}
