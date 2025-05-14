#include <stdint.h>
#include "kheap.h"

#define KERNEL_HEAP_START 0x1000000  // 16MB
#define KERNEL_HEAP_SIZE  0x100000   // 1MB

static uint32_t heap_end = KERNEL_HEAP_START;

void* kmalloc(uint32_t size) {
    void* addr = (void*)heap_end;
    heap_end += size;

    // Align heap_end to next 4-byte boundary
    if (heap_end & 0x3) {
        heap_end = (heap_end + 4) & ~0x3;
    }

    return addr;
}
