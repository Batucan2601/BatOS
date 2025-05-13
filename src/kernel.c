extern void init_gdt();
extern void init_paging();

void kernel_main() {
    init_gdt();  // Set up GDT first
    init_paging();
    char* vga = (char*)0xb8000;
    const char* msg = "GDT  and paging Loaded: BatOS!";
    for (int i = 0; msg[i] != '\0'; ++i) {
        vga[i * 2] = msg[i];
        vga[i * 2 + 1] = 0x07;
    }
}
