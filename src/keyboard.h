#pragma once 
#include <stdint.h>
#define KEYBOARD_BUFFER_SIZE 1024

struct Keyboard
{
    uint8_t keyboard_array[KEYBOARD_BUFFER_SIZE];
    uint32_t keyboard_counter;
};

void keyboard_init();