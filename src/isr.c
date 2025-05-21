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


void irq_handler(uint32_t int_no) {
    if (int_no == 32) {
        // No printing inside interrupts for now!
    }
    if (int_no == 33) 
    {
        uint8_t scancode = inb(0x60);  // Read scancode from keyboard controller
        on_keyboard_pressed(scancode);    
    }

    // Send End of Interrupt (EOI)
    if (int_no >= 40) outb(0xA0, 0x20);  // Slave PIC
    outb(0x20, 0x20);                    // Master PIC
}


void isr_handler(uint32_t int_no, uint32_t err_code) {

    if (int_no >= 32 && int_no <= 47) {
        irq_handler(int_no);
        return;  // ✅ RETURN — do not halt
    }
    print("\nISR fired!\nInterrupt number: ");
    print_hex(int_no);
    print("\nError code: ");
    print_hex(err_code);
    print(" (Exception)\n");

    if (int_no < 32)
        print(exception_messages[int_no]);
    else
        print("Unknown");

    print("\nSystem halted due to exception.\n");

    while (1) __asm__("hlt");  // ❌ Only here!
}