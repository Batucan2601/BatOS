/* link.ld */
ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .text : {
        *(.multiboot)    /* Must come first! */
        *(.text*)
    }

    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) }
}