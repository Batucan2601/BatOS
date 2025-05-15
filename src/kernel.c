#include "kheap.h"
#include "idt.h"
#include "vga.h"

void kernel_main() {
    init_gdt();
    init_idt();  // new
    init_paging();

    print("Allocated 100 bytes at: ");
    int a = 1 / 0;  // Division by zero — triggers ISR 0
}