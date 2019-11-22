// kernel entry
#include "console.h"

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
}
