#include "keyboard.h"
#include "filesystemcommands.h"
#include "vga.h"

static struct Keyboard keyboard;


static const char scancode_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*',
    0, ' '
};

void keyboard_init()
{
    for (uint16_t i = 0; i < KEYBOARD_BUFFER_SIZE; i++)
    {
        keyboard.keyboard_array[i] = 0;
    }
    keyboard.keyboard_counter = 0;
}


void on_keyboard_pressed(uint8_t scancode )
{
    if( scancode > 128 )
    {
        return; 
    }
    if (scancode == 0x0E) 
    {
        if (keyboard.keyboard_counter > 0) 
        {
            keyboard.keyboard_counter--;
            keyboard.keyboard_array[keyboard.keyboard_counter] = '\0';
            vga_backspace();
        }
    }
    else if( scancode == 0x1C ) // enter 
    {
        vga_newkey('\n');
        keyboard.keyboard_array[keyboard.keyboard_counter] = '\0';
        execute_command(keyboard.keyboard_array);
        keyboard.keyboard_counter = 0; 
    }
    else if( scancode == 0x39 )
    {
        keyboard.keyboard_array[keyboard.keyboard_counter++] = ' ';
        vga_newkey(' ');
    }
    else
    {
        keyboard.keyboard_array[keyboard.keyboard_counter++] = scancode_map[scancode];
        vga_newkey(scancode_map[scancode]);
    }
}