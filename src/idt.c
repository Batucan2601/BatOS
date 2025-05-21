#include "idt.h"

extern void idt_flush(uint32_t);
extern void isr_stub_0();  // ← Declare this
extern void isr_stub_1();  // ← Declare this
extern void isr_stub_2();  // ← Declare this
extern void isr_stub_3();  // ← Declare this
extern void isr_stub_4();  // ← Declare this
extern void isr_stub_5();  // ← Declare this
extern void isr_stub_6();  // ← Declare this
extern void isr_stub_7();  // ← Declare this
extern void isr_stub_8();  // ← Declare this
extern void isr_stub_9();  // ← Declare this
extern void isr_stub_10(); // ← Declare this
extern void isr_stub_11(); // ← Declare this
extern void isr_stub_12(); // ← Declare this
extern void isr_stub_13(); // ← Declare this
extern void isr_stub_14(); // ← Declare this
extern void isr_stub_15(); // ← Declare this
extern void isr_stub_16(); // ← Declare this
extern void isr_stub_17(); // ← Declare this
extern void isr_stub_18(); // ← Declare this
extern void isr_stub_19(); // ← Declare this
extern void isr_stub_20(); // ← Declare this
extern void isr_stub_21(); // ← Declare this
extern void isr_stub_22(); // ← Declare this
extern void isr_stub_23(); // ← Declare this
extern void isr_stub_24(); // ← Declare this
extern void isr_stub_25(); // ← Declare this
extern void isr_stub_26(); // ← Declare this
extern void isr_stub_27(); // ← Declare this
extern void isr_stub_28(); // ← Declare this
extern void isr_stub_29(); // ← Declare this
extern void isr_stub_30(); // ← Declare this
extern void isr_stub_31(); // ← Declare this
extern void isr_stub_32(); // ← Declare this
extern void isr_stub_33(); // ← Declare this
extern void isr_stub_34(); // ← Declare this
extern void isr_stub_35(); // ← Declare this
extern void isr_stub_36(); // ← Declare this
extern void isr_stub_37(); // ← Declare this
extern void isr_stub_38(); // ← Declare this
extern void isr_stub_39(); // ← Declare this
extern void isr_stub_40(); // ← Declare this
extern void isr_stub_41(); // ← Declare this
extern void isr_stub_42(); // ← Declare this
extern void isr_stub_43(); // ← Declare this
extern void isr_stub_44(); // ← Declare this
extern void isr_stub_45(); // ← Declare this
extern void isr_stub_46(); // ← Declare this
extern void isr_stub_47(); // ← Declare this
extern void isr_stub_48(); // ← Declare this

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idtp;
extern void* isr_stubs[];


void set_idt_gate(int n, uint32_t handler) {
    idt[n].base_low = handler & 0xFFFF;
    idt[n].sel = 0x08;  // Kernel code segment selector
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; // Present | Ring 0 | 32-bit interrupt gate
    idt[n].base_high = (handler >> 16) & 0xFFFF;
}

void init_idt() {
    idtp.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;

    // 💥 Just this for now — manually install only ISR 0
    set_idt_gate(0, (uint32_t)isr_stub_0);
    set_idt_gate(1, (uint32_t)isr_stub_1);
    set_idt_gate(2, (uint32_t)isr_stub_2);
    set_idt_gate(3, (uint32_t)isr_stub_3);
    set_idt_gate(4, (uint32_t)isr_stub_4);
    set_idt_gate(5, (uint32_t)isr_stub_5);
    set_idt_gate(6, (uint32_t)isr_stub_6);
    set_idt_gate(7, (uint32_t)isr_stub_7);
    set_idt_gate(8, (uint32_t)isr_stub_8);
    set_idt_gate(9, (uint32_t)isr_stub_9);
    set_idt_gate(10, (uint32_t)isr_stub_10);
    set_idt_gate(11, (uint32_t)isr_stub_11);
    set_idt_gate(12, (uint32_t)isr_stub_12);
    set_idt_gate(13, (uint32_t)isr_stub_13);
    set_idt_gate(14, (uint32_t)isr_stub_14);
    set_idt_gate(15, (uint32_t)isr_stub_15);
    set_idt_gate(16, (uint32_t)isr_stub_16);
    set_idt_gate(17, (uint32_t)isr_stub_17);
    set_idt_gate(18, (uint32_t)isr_stub_18);
    set_idt_gate(19, (uint32_t)isr_stub_19);
    set_idt_gate(20, (uint32_t)isr_stub_20);
    set_idt_gate(21, (uint32_t)isr_stub_21);
    set_idt_gate(22, (uint32_t)isr_stub_22);
    set_idt_gate(23, (uint32_t)isr_stub_23);
    set_idt_gate(24, (uint32_t)isr_stub_24);
    set_idt_gate(25, (uint32_t)isr_stub_25);
    set_idt_gate(26, (uint32_t)isr_stub_26);
    set_idt_gate(27, (uint32_t)isr_stub_27);
    set_idt_gate(28, (uint32_t)isr_stub_28);
    set_idt_gate(29, (uint32_t)isr_stub_29);
    set_idt_gate(30, (uint32_t)isr_stub_30);
    set_idt_gate(31, (uint32_t)isr_stub_31);
    set_idt_gate(32, (uint32_t)isr_stub_32);
    set_idt_gate(33, (uint32_t)isr_stub_33);
    set_idt_gate(34, (uint32_t)isr_stub_34);
    set_idt_gate(35, (uint32_t)isr_stub_35);
    set_idt_gate(36, (uint32_t)isr_stub_36);
    set_idt_gate(37, (uint32_t)isr_stub_37);
    set_idt_gate(38, (uint32_t)isr_stub_38);
    set_idt_gate(39, (uint32_t)isr_stub_39);
    set_idt_gate(40, (uint32_t)isr_stub_40);
    set_idt_gate(41, (uint32_t)isr_stub_41);
    set_idt_gate(42, (uint32_t)isr_stub_42);
    set_idt_gate(43, (uint32_t)isr_stub_43);
    set_idt_gate(44, (uint32_t)isr_stub_44);
    set_idt_gate(45, (uint32_t)isr_stub_45);
    set_idt_gate(46, (uint32_t)isr_stub_46);
    set_idt_gate(47, (uint32_t)isr_stub_47);

   
    idt_flush((uint32_t)&idtp);
}
