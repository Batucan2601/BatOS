#pragma once

#include <stdint.h>

void print(const char* str);
void print_hex(uint32_t value);
void print_char(char c);
void move_cursor();
void vga_backspace();
void vga_newkey(uint8_t scancode);
void vga_write_directory(char* str);