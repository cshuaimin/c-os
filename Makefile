CC	=gcc
CFLAGS	=-Os -m32 -Wall -g -fno-pic -nostartfiles -ffreestanding -fno-builtin -fno-stack-protector -nostdinc -Wno-unused-function
LDFLAGS	=-m elf_i386 -N -nostdlib

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

all: img

img: bootsector kernel
	cp bootsector img
	cat kernel >> img

bootsector: boot.o loadelf.o tobootsector
	# Be sure that `boot.o' is the first file!
	ld $(LDFLAGS) -Ttext 0x7c00 boot.o loadelf.o -o boot.elf
	objcopy -SO binary boot.elf boot.bin
	./tobootsector boot.bin bootsector

boot.o: boot.S asm.h
loadelf.o: loadelf.c asmhelp.h
kernel.o: kernel.c asmhelp.h

kernel: kernel.o
	ld $(LDFLAGS) -s -Ttext 0x40000 -e kernel_main kernel.o -o kernel

tobootsector: tobootsector.c
	$(CC) -Wall tobootsector.c -o tobootsector

dbg: img
	gnome-terminal -e 'gdb -q -tui -x gdb-qemu'
	qemu-system-i386 -hda img -S -s -monitor stdio

run: img
	qemu-system-i386 -hda img -monitor stdio

