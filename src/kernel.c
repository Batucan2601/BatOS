#include "kheap.h"
#include "idt.h"
#include "vga.h"

void kernel_main() {
   init_gdt();
   init_idt(); // contains set_idt_gate(0, (uint32_t)isr_stub_0)
   print("About to trigger INT 0\n");
   int a = 1 /0 ;
   //__asm__ volatile ("int $0");
   //print("Should never reach here\n");
}