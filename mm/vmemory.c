#include "vmemory.h"

#include "memory.h"
#include "common.h"

// page drictory
pgd_t pgd_kern[PGD_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

// page table
static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__ ((aligned(PAGE_SIZE)));

void vmm_init()
{
	// 0xC0000000, page dirctory index
	u32int kern_pte_first_idx = PGD_INDEX(PAGE_OFFSET);
	u32int length = PTE_COUNT + kern_pte_first_idx;

	u32int i, j;
	for (i = kern_pte_first_idx, j = 0; i < length; i++, j++) {
		// sub PAGE_OFFSET to get physical memory address 
		pgd_kern[i] = ((u32int)pte_kern[j] - PAGE_OFFSET) | PAGE_PRESENT | PAGE_WRITE;
	}

	u32int *pte = (u32int *)pte_kern;
	for (i = 1; i < PTE_COUNT * PTE_SIZE; i++) {
		pte[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	u32int pgd_kern_phy_addr = (u32int)pgd_kern - PAGE_OFFSET;

	// register page error interrupt
	//register_interrupt_handler(14, &page_fault);

	switch_pgd(pgd_kern_phy_addr);
}

void switch_pgd(u32int pd) {
	asm volatile ("mov %0, %%cr3" : : "r" (pd));
}

void map(pgd_t *pgd_now, u32int va, u32int pa, u32int flags)
{ 	
	u32int pgd_idx = PGD_INDEX(va);
	u32int pte_idx = PTE_INDEX(va); 
	
	pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
	if (!pte) {
		pte = (pte_t *)mem_page_alloc();
		pgd_now[pgd_idx] = (u32int)pte | PAGE_PRESENT | PAGE_WRITE;

		// trans to linear addr
		pte = (pte_t *)((u32int)pte + PAGE_OFFSET);
		bzero(pte, PAGE_SIZE);
	} else {
		pte = (pte_t *)((u32int)pte + PAGE_OFFSET);
	}

	pte[pte_idx] = (pa & PAGE_MASK) | flags;

	// update page table cache
	asm volatile ("invlpg (%0)" : : "a" (va));
}

void unmap(pgd_t *pgd_now, u32int va)
{
	u32int pgd_idx = PGD_INDEX(va);
	u32int pte_idx = PTE_INDEX(va);

	pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);

	if (!pte) {
		return;
	}

	pte = (pte_t *)((u32int)pte + PAGE_OFFSET);
	pte[pte_idx] = 0;

	asm volatile ("invlpg (%0)" : : "a" (va));
}

u32int get_mapping(pgd_t *pgd_now, u32int va, u32int *pa)
{
	u32int pgd_idx = PGD_INDEX(va);
	u32int pte_idx = PTE_INDEX(va);

	pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
	if (!pte) {
	      return 0;
	}
	
	pte = (pte_t *)((u32int)pte + PAGE_OFFSET);
	if (pte[pte_idx] != 0 && pa) {
		 *pa = pte[pte_idx] & PAGE_MASK;
		return 1;
	}

	return 0;
}

