#include <stdint.h>
#include "kheap.h"

struct HeapAttributes {
    uint32_t start;
    uint32_t size;
} __attribute__((packed));

struct HeapStorage {
    struct HeapAttributes attributes[MAXIMUM_NO_OF_ALLOC];
    uint32_t allocated;
}__attribute__((packed));

struct HeapStorage heap_storage = {0}; 
static uint32_t heap_end = KERNEL_HEAP_START;

void* kmalloc(uint32_t size) {
    void* addr = (void*)heap_end;
    heap_end += size;

    // Align heap_end to next 4-byte boundary
    if (heap_end & 0x3) { //quick way to check division by 4 
        heap_end = (heap_end + 4) & ~0x3;
    }
    heap_storage.attributes[heap_storage.allocated].start = (uint32_t)addr;
    heap_storage.attributes[heap_storage.allocated].size = size;
    heap_storage.allocated++;
    return addr;
}

void kfree(void* ptr) {
    for (uint32_t i = 0; i < heap_storage.allocated; i++)
    {
        if (heap_storage.attributes[i].start == (uint32_t)ptr) {
            // Mark the block as free
            heap_storage.attributes[i].start = 0;
            heap_storage.attributes[i].size = 0;
            break;
        }
    }
}