#include "isr.h"
#include "vga.h"

void isr_handler(uint32_t int_no) {
    print("INT: ");
    print_hex(int_no);
    print("\n");
}
