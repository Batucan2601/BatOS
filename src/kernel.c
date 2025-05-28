#include "kheap.h"
#include "idt.h"
#include "vga.h"
#include "PIC.h"
#include "ports.h"
#include "pit.h"
#include "keyboard.h"
#include "filesystem.h"
#include "fs_disk.h"
#include "filesystemcommands.h"
#include "ide.h"


void kernel_main() {
   __asm__ volatile("cli");
   print("hello kernel\n");
   init_gdt();
   init_paging();
   init_idt();
   pic_remap();
   init_pit(100);
   keyboard_init();
   print("Keyboard initialized\n");
   fs_init();
   print("fs init \n");
   fs_disk_init();
   print("fs disk \n");

   show_directory();

   // irq stuff
   outb(0x21, 0xFE);
   outb(0xA1, 0xFF);

   outb(0x21, inb(0x21) & ~0x02);  // Unmask IRQ1 (bit 1 = 0)
   __asm__ volatile("sti");
   
    uint8_t buffer[512];
    ide_read_sector(0, buffer);

    print("First 16 bytes of sector 0:\n");
    for (int i = 0; i < 16; i++) {
        print_hex(buffer[i]);
        print(" ");
    }
    
   while(1)
   __asm__ volatile("hlt");

}

