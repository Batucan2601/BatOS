#pragma once
#include <stdint.h>

#define IDT_ENTRIES 256

typedef struct {
    uint16_t base_low;
    uint16_t sel;       // Kernel segment selector
    uint8_t  always0;   // Always 0
    uint8_t  flags;     // Type and attributes
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

void init_idt();
