#include "vga.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F
#define LIGHT_BLUE_ON_BLACK 0x09

static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void put_entry_at(char c, uint8_t color, uint8_t x, uint8_t y) {
    const uint32_t index = y * VGA_WIDTH + x;
    vga_buffer[index] = ((uint16_t)color << 8) | c;
}

void print_char(char c) {
    if (c == '\n') {
        cursor_y++;
        cursor_x = 0;
        return;
    }

    put_entry_at(c, WHITE_ON_BLACK, cursor_x, cursor_y);
    cursor_x++;

    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = 0;
    }
}
vga_write_directory(char* str)
{
    for (uint32_t i = 0; str[i] != '\0'; ++i) 
    {
        put_entry_at(str[i], LIGHT_BLUE_ON_BLACK, cursor_x, cursor_y);
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
        if (cursor_y >= VGA_HEIGHT) 
        {
            cursor_y = 0;
        }
        
    }
    put_entry_at(':', LIGHT_BLUE_ON_BLACK, cursor_x, cursor_y);
    cursor_x++;
    // this is for aesthethic reasons
    cursor_x++;

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

void move_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_newkey(uint8_t c) {
    if (c == '\n') {
        cursor_y++;
        cursor_x = 0;
    } else {
        vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = (WHITE_ON_BLACK << 8) | c;
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = 0;
    }

    move_cursor();
}

void vga_backspace() {
    if (cursor_x == 0 && cursor_y == 0)
        return;

    if (cursor_x == 0) {
        cursor_y--;
        cursor_x = VGA_WIDTH - 1;
    } else {
        cursor_x--;
    }
    vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = ' ' | (0x07 << 8);  // clear char
    move_cursor();
}
