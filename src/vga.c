#include "vga.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F

static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

static void put_entry_at(char c, uint8_t color, uint8_t x, uint8_t y) {
    const uint32_t index = y * VGA_WIDTH + x;
    vga_buffer[index] = ((uint16_t)color << 8) | c;
}

void print_char(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
        return;
    }

    put_entry_at(c, WHITE_ON_BLACK, cursor_col, cursor_row);
    cursor_col++;

    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
    }

    if (cursor_row >= VGA_HEIGHT) {
        cursor_row = 0;
    }
}

void print(const char* str) {
    for (uint32_t i = 0; str[i] != '\0'; ++i) {
        print_char(str[i]);
    }
}

void print_hex(uint32_t value) {
    const char* hex = "0123456789ABCDEF";
    print("0x");
    for (int i = 28; i >= 0; i -= 4) {
        print_char(hex[(value >> i) & 0xF]);
    }
}
