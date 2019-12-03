#include "memory.h"
#include "kernel.h"
#include "multiboot.h"

// memory stack
static u32int MEM_STACK[PAGE_MAX_SIZE + 1];
// memory stack top index
static u32int MEM_STACK_TOP;

void mem_show()
{
    printk("Kernel Memory Info:\n");
    printk("kernel starts at:\t0x%08X\n", kern_start);
    printk("kernel ends at:\t\t0x%08X\n", kern_end);
    printk("kernel used:\t\t%dKB\n\n", (kern_end - kern_start)/1024);

    mmap_entry_t* mmap = (mmap_entry_t*)glb_mboot_ptr->mmap_addr;
    u32int mmap_len = (u32int)mmap + glb_mboot_ptr->mmap_length;

    printk("Physical Memory Map:\n");
    for(; (u32int)mmap < mmap_len; mmap++)
    {
        printk("base_addr = 0x%X%08X, length = 0x%X%08X, type = 0x%X\n",  
                (u32int)mmap->base_addr_high, (u32int)mmap->base_addr_low,
                (u32int)mmap->length_high, (u32int)mmap->length_low,
                (u32int)mmap->type);                                        
    }
}

void mem_init()
{
    u32int base_addr = 0x100000;
    mmap_entry_t* mmap_start = (mmap_entry_t*)glb_mboot_ptr->mmap_addr;
    mmap_entry_t* mmap_end = mmap_start + glb_mboot_ptr->mmap_length;

    // init all available physical memory
    for(; mmap_start < mmap_end; mmap_start++)
    {
        // use $base_addr + length at present
        if(mmap_start->type == 1 && mmap_start->base_addr_low == base_addr)
        {
            u32int page_addr = base_addr + (kern_end - kern_start); 
            u32int length = base_addr + mmap_start->length_low;

            while(page_addr < length && page_addr <= PMM_MAX_SIZE)
            {
                mem_page_free(page_addr);
                page_addr += PMM_PAGE_SIZE;
            }
        }
    }
}

u32int mem_page_alloc()                                            
{                     
    if (MEM_STACK_TOP == 0) 
        return 0;

    u32int page = MEM_STACK[MEM_STACK_TOP--];                      

    return page;      
}                     

void mem_page_free(u32int addr)                                       
{                     
    if (MEM_STACK_TOP == PAGE_MAX_SIZE)
        return;
    MEM_STACK[++MEM_STACK_TOP] = addr;                                  
}                     

