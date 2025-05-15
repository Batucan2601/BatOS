#pragma once
#include <stdint.h>

typedef struct {
    uint32_t int_no;
} isr_regs_t;

void isr_handler(uint32_t int_no);
