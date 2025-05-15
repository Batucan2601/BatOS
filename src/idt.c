#include "idt.h"

extern void isr_stub_table(); // We’ll define it in assembly

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idtp;

extern void idt_flush(uint32_t);  // Assembly routine

void set_idt_gate(int n, uint32_t handler) {
    idt[n].base_low = handler & 0xFFFF;
    idt[n].sel = 0x08;  // Kernel code segment
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; // Present | Ring 0 | 32-bit interrupt gate
    idt[n].base_high = (handler >> 16) & 0xFFFF;
}

void init_idt() {
    idtp.limit = (sizeof(idt_entry_t) * IDT_ENTRIES) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < 32; i++) {
        extern void* isr_stubs[];
        set_idt_gate(i, (uint32_t)isr_stubs[i]);
    }

    idt_flush((uint32_t)&idtp);
}
