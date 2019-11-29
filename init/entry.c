// kernel entry
#include "console.h"
#include "descriptor_tables.h"
#include "timer.h"

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

    asm volatile("sti");
    init_timer(50);
}
