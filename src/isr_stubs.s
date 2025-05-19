[bits 32]

global isr_stubs
extern isr_handler

section .text

isr_stubs:
%assign i 0
%rep 32
    %define stub isr_stub_%+i
    global stub         ; 🔥 make stub visible to C
    stub:
        cli
        push 0
        push dword i
        pusha
        call isr_handler
        add esp, 8
        popa
        sti
        iret
    dd stub             ; 🔥 add to isr_stubs[]
%assign i i+1
%endrep
