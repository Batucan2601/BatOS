#include "kheap.h"
#include "idt.h"
#include "vga.h"
#include "PIC.h"
#include "ports.h"
#include "pit.h"

void kernel_main() {
      __asm__ volatile("cli");
   print("hello kernel\n");
   init_gdt();
   init_paging();
   init_idt();
   pic_remap();
   init_pit(100);
   outb(0x21, 0xFE);
   outb(0xA1, 0xFF);
   __asm__ volatile("sti");
   
   while(1)
   __asm__ volatile("hlt");

}

