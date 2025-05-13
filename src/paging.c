#include <stdint.h>

#define PAGE_SIZE 4096
#define PAGE_ENTRIES 1024

uint32_t page_directory[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));
uint32_t first_page_table[PAGE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

void init_paging() {
    for (uint32_t i = 0; i < PAGE_ENTRIES; ++i) {
        first_page_table[i] = (i * PAGE_SIZE) | 3; // Present + Read/Write
    }

    page_directory[0] = ((uint32_t)first_page_table) | 3;
    for (uint32_t i = 1; i < PAGE_ENTRIES; ++i) {
        page_directory[i] = 0; // Unused
    }

    asm volatile (
        "mov %0, %%cr3\n"     // Load page directory address into CR3
        "mov %%cr0, %%eax\n"
        "or $0x80000000, %%eax\n" // Set PG bit (bit 31) in CR0
        "mov %%eax, %%cr0\n"
        : : "r"(page_directory) : "eax"
    );
}
