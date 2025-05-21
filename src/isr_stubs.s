[bits 32]

global isr_stubs
extern isr_handler

section .text

isr_stubs:
%assign i 0
%rep 48               ; 0–31 = ISRs, 32–47 = IRQs
    %define stub isr_stub_%+i
    global stub
    stub:
        cli
        pusha                      ; save all general purpose registers
        push dword 0               ; push error code
        push dword i              ; push interrupt number
        call isr_handler
        add esp, 8                 ; clean up args
        popa                       ; restore registers
        sti
        iret
    dd stub
%assign i i+1
%endrep
