unset GTK_PATH
rm  bin/*.o BatOS.iso iso/boot/kernel.elf
gcc -m32 -ffreestanding -c src/kernel.c -o bin/kernel.o
gcc -m32 -ffreestanding -c src/gdt.c -o bin/gdt.o
gcc -m32 -ffreestanding -c src/paging.c -o bin/paging.o
gcc -m32 -ffreestanding -c src/kheap.c -o bin/kheap.o
gcc -m32 -ffreestanding -c src/vga.c -o bin/vga.o
gcc -m32 -ffreestanding -c src/isr.c -o bin/isr.o
gcc -m32 -ffreestanding -c src/idt.c -o bin/idt.o
gcc -m32 -ffreestanding -c src/PIC.c -o bin/PIC.o
gcc -m32 -ffreestanding -c src/ports.c -o bin/ports.o
gcc -m32 -ffreestanding -c src/pit.c -o bin/pit.o
gcc -m32 -ffreestanding -c src/keyboard.c -o bin/keyboard.o
gcc -m32 -ffreestanding -c src/filesystem.c -o bin/filesystem.o
gcc -m32 -ffreestanding -c src/stringcustom.c -o bin/stringcustom.o
gcc -m32 -ffreestanding -c src/filesystemcommands.c -o bin/filesystemcommands.o


nasm -f elf32 src/kernel_entry.s -o bin/kernel_entry.o
nasm -f elf32 src/idt_flush.s -o bin/idt_flush.o
nasm -f elf32 src/isr_stubs.s -o bin/isr_stubs.o

gcc -m32 -ffreestanding -c src/keyboard.c -o bin/keyboard.o
ld -m elf_i386 -T link.ld -o bin/kernel.elf bin/kernel_entry.o bin/isr_stubs.o bin/idt_flush.o bin/kernel.o bin/gdt.o bin/paging.o bin/kheap.o bin/vga.o bin/isr.o bin/idt.o  bin/PIC.o bin/ports.o bin/pit.o  bin/keyboard.o bin/filesystem.o bin/stringcustom.o bin/filesystemcommands.o -o bin/kernel.elf 
cp bin/kernel.elf iso/boot/kernel.elf
grub-mkrescue -o BatOS.iso iso

qemu-system-x86_64 -cdrom BatOS.iso 
