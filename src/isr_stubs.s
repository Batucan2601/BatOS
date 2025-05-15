[bits 32]
global isr_stubs
global isr_stub_table
extern isr_handler

isr_stubs:
isr_stub_table:

%assign i 0
%rep 32
    isr_stub_%+i:
        cli
        push dword i        ; Push interrupt number
        call isr_handler
        add esp, 4          ; Clean up pushed interrupt number
        sti
        iret
    dd isr_stub_%+i
%assign i i+1
%endrep
