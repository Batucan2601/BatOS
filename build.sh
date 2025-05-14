unset GTK_PATH
gcc -m32 -ffreestanding -c src/kernel.c -o bin/kernel.o
gcc -m32 -ffreestanding -c src/gdt.c -o bin/gdt.o
gcc -m32 -ffreestanding -c src/paging.c -o bin/paging.o
gcc -m32 -ffreestanding -c src/kheap.c -o bin/kheap.o
nasm -f elf32 src/kernel_entry.s -o bin/kernel_entry.o

ld -m elf_i386 -T link.ld -o bin/kernel.elf bin/kernel_entry.o bin/kernel.o bin/gdt.o bin/paging.o bin/kheap.o

cp bin/kernel.elf iso/boot/kernel.elf
grub-mkrescue -o BatOS.iso iso

qemu-system-x86_64 -cdrom BatOS.iso
