#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>

#define KERNEL_HEAP_START 0x1000000  // 16MB
#define KERNEL_HEAP_SIZE  0x100000   // 1MB
#define MAXIMUM_NO_OF_ALLOC 0x1000 // 4KB


void* kmalloc(uint32_t size);

#endif