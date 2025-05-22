#include "kheap.h"
#include "idt.h"
#include "vga.h"
#include "PIC.h"
#include "ports.h"
#include "pit.h"
#include "keyboard.h"
#include "filesystem.h"
#include "filesystemcommands.h"


void kernel_main() {
      __asm__ volatile("cli");
   print("hello kernel\n");
   init_gdt();
   init_paging();
   init_idt();
   pic_remap();
   init_pit(100);
   keyboard_init();
   init_fs();
   show_directory();
   outb(0x21, 0xFE);
   outb(0xA1, 0xFF);

   outb(0x21, inb(0x21) & ~0x02);  // Unmask IRQ1 (bit 1 = 0)
   __asm__ volatile("sti");
   
   while(1)
   __asm__ volatile("hlt");

}

