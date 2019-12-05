#ifndef INCLUDE_VMM_H
#define INCLUDE_VMM_H

#include "common.h"

// virtual mem offset 
#define PAGE_OFFSET 	0xC0000000

#define PAGE_PRESENT 	0x1
#define PAGE_WRITE 	    0x2
#define PAGE_USER   	0x4

// memory page size
#define PAGE_SIZE   	4096
// align code
#define PAGE_MASK      0xFFFFF000

#define PGD_INDEX(x) (((x) >> 22) & 0x3FF)
#define PTE_INDEX(x) (((x) >> 12) & 0x3FF)
#define OFFSET_INDEX(x) ((x) & 0xFFF)

typedef u32int pgd_t;
typedef u32int pte_t;

#define PGD_SIZE (PAGE_SIZE/sizeof(pte_t))
#define PTE_SIZE (PAGE_SIZE/sizeof(u32int))

// 512M page tables count, 512M / 4M = 128
#define PTE_COUNT 128

extern pgd_t pgd_kern[PGD_SIZE];

void vmm_init();
void switch_pgd(u32int pd);
void map(pgd_t *pgd_now, u32int va, u32int pa, u32int flags);
void unmap(pgd_t *pgd_now, u32int va);
u32int get_mapping(pgd_t *pgd_now, u32int va, u32int *pa);
//void page_fault(pt_regs *regs);

#endif
