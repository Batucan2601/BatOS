#include <stdint.h>

struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct GDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct GDTEntry gdt[3];
struct GDTPointer gdt_ptr;

void set_gdt_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[i].base_low    = base & 0xFFFF;
    gdt[i].base_middle = (base >> 16) & 0xFF;
    gdt[i].base_high   = (base >> 24) & 0xFF;

    gdt[i].limit_low   = limit & 0xFFFF;
    gdt[i].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[i].access      = access;
}

void init_gdt() {
    gdt_ptr.limit = (sizeof(struct GDTEntry) * 3) - 1;
    gdt_ptr.base  = (uint32_t)&gdt;

    set_gdt_entry(0, 0, 0, 0, 0);                // Null segment
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Code segment
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Data segment

    extern void gdt_flush(uint32_t);
    gdt_flush((uint32_t)&gdt_ptr);
}
