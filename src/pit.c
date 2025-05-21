#include "ports.h"
#include "pit.h"
void init_pit(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;
    outb(0x43, 0x36);                  // channel 0, low+high, mode 3
    outb(0x40, divisor & 0xFF);        // low byte
    outb(0x40, (divisor >> 8) & 0xFF); // high byte
}