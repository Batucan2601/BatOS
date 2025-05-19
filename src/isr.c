#include "vga.h"
#include "isr.h"

static const char* exception_messages[32] = {
    "Division by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Overflow",
    "Bound range exceeded",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack-segment fault",
    "General protection fault",
    "Page fault",
    "Reserved",
    "x87 floating-point exception",
    "Alignment check",
    "Machine check",
    "SIMD floating-point exception",
    "Virtualization exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(uint32_t int_no, uint32_t err_code) {
    print("\nEXCEPTION: ");
    if (int_no < 32)
        print(exception_messages[int_no]);
    else
        print("Unknown exception");

    print("\nInterrupt number: ");
    print_hex(int_no);

    print("\nError code: ");
    print_hex(err_code);

    while (1) __asm__("hlt");
}
