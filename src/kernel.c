#include "kheap.h"

void kernel_main() {
    init_gdt();
    init_paging();

    char* vga = (char*)0xb8000;
    const char* msg = "Paging + Heap ready";
    for (int i = 0; msg[i] != '\0'; ++i) {
        vga[i * 2] = msg[i];
        vga[i * 2 + 1] = 0x07;
    }

    // Test allocation
    char* test = (char*)kmalloc(100);
    test[0] = 'X';
    test[1] = '\0';
}