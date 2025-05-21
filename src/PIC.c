#include "ports.h"
#include "PIC.h"
#include "vga.h"

#define PIC1        0x20
#define PIC2        0xA0
#define PIC1_DATA   0x21
#define PIC2_DATA   0xA1
#define ICW1_INIT   0x11
#define ICW4_8086   0x01

static void io_wait() {
    outb(0x80, 0);  // Dummy write to port 0x80 (used for delay)
}

void pic_remap() {
    outb(PIC1, ICW1_INIT);
    io_wait();
    outb(PIC2, ICW1_INIT);
    io_wait();

    outb(PIC1_DATA, 0x20);  // Remap master to 0x20 (32)
    io_wait();
    outb(PIC2_DATA, 0x28);  // Remap slave to 0x28 (40)
    io_wait();

    outb(PIC1_DATA, 4);     // Tell master that slave is at IRQ2
    io_wait();
    outb(PIC2_DATA, 2);     // Tell slave its cascade identity
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    // Mask all IRQs initially
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

