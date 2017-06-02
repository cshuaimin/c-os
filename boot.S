#include "asm.h"

    .code16
    .section .text
    .global _start

_start:
    cli
    # cld

    xorw %ax, %ax
    # movw %ax, %ss
    movw %ax, %ds
    movw %ax, %es

    # A20
    inb $0x92, %al
    orb $0b10, %al
    outb %al, $0x92

    lgdt gdtdesc
    movl %cr0, %eax
    orl $1, %eax
    movl %eax, %cr0
    ljmp $0b1000, $pmode

.code32
pmode:
    movw $0b10000, %ax              # Our data segment selector
    movw %ax, %ds                   # -> DS: Data Segment
    movw %ax, %es                   # -> ES: Extra Segment
    movw %ax, %fs                   # -> FS
    movw %ax, %gs                   # -> GS
    movw %ax, %ss                   # -> SS: Stack Segment

    xorl %ebp, %ebp
    movl $_start, %esp
    call bootmain

    hlt

.p2align 2                      # force 4 byte alignment
gdt:
    SEG_NULLASM                     # null seg
    SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff)       # code seg for bootloader and kernel
    SEG_ASM(STA_W, 0x0, 0xffffffff)         # data seg for bootloader and kernel

gdtdesc:
    .word 0x17                      # sizeof(gdt) - 1
    .long gdt                       # address gdt
