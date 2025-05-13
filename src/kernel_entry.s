; src/kernel_entry.s
[bits 32]
[extern kernel_main]

section .multiboot
align 4
    dd 0x1BADB002          ; magic number
    dd 0x0                ; flags
    dd -(0x1BADB002 + 0x0) ; checksum

section .text
global _start
_start:
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang
; Flush GDT
global gdt_flush
gdt_flush:
    mov eax, [esp + 4]
    lgdt [eax]

    ; Reload segment registers
    mov ax, 0x10     ; Data segment (index 2 in GDT)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush_done  ; Code segment (index 1 in GDT)
.flush_done:
    ret
