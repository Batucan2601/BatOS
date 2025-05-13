void kernel_main() {
    char* vga = (char*)0xb8000;
    const char* msg = "Welcome to BatOS!";
    for (int i = 0; msg[i] != '\0'; ++i) {
        vga[i * 2] = msg[i];
        vga[i * 2 + 1] = 0x07;  // white on black
    }
}