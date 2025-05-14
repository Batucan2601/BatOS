#include <stdint.h>
#include "kheap.h"
#define PAGE_SIZE       0x1000      // 4 KB
#define PAGE_ENTRIES    1024

// Page tables and directory
uint32_t page_directory[PAGE_ENTRIES]    __attribute__((aligned(PAGE_SIZE)));
uint32_t first_page_table[PAGE_ENTRIES]  __attribute__((aligned(PAGE_SIZE))); // 0–4MB
uint32_t heap_page_table[PAGE_ENTRIES]   __attribute__((aligned(PAGE_SIZE))); // 16–17MB

void init_paging() {
    // -------------------------------
    // Map 0x00000000–0x003FFFFF (first 4MB)
    // -------------------------------
    for (uint32_t i = 0; i < PAGE_ENTRIES; ++i) {
        first_page_table[i] = (i * PAGE_SIZE) | 3; // Present | RW
    }

    // -------------------------------
    // Map 0x1000000–0x10FFFFF (heap: 16MB–17MB)
    // -------------------------------
    for (uint32_t i = 0; i < PAGE_ENTRIES; ++i) {
        heap_page_table[i] = (KERNEL_HEAP_START + i * PAGE_SIZE) | 3; // Present | RW
    }

    // -------------------------------
    // Build page directory
    // -------------------------------
    
    // Clear 
    for (uint32_t i = 0; i < PAGE_ENTRIES; ++i) {
        page_directory[i] = 0;
    }
    // First entry: 0–4MB
    page_directory[0] = ((uint32_t)first_page_table) | 3;

    // Heap entry: 16–20MB → 16MB / 4MB = index 4
    page_directory[4] = ((uint32_t)heap_page_table) | 3;

   
    // -------------------------------
    // Load CR3 with page directory, enable PG in CR0
    // -------------------------------
    asm volatile (
        "mov %0, %%cr3\n"           // Load page directory base
        "mov %%cr0, %%eax\n"
        "or $0x80000000, %%eax\n"   // Set PG (paging enable) bit
        "mov %%eax, %%cr0\n"
        :
        : "r"(page_directory)
        : "eax"
    );
}
