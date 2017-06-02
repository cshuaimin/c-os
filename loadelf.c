#include "asmhelp.h"
#include "elf.h"

void __always_inline
read_sector(uint16_t sect, uint16_t cyl, uint8_t head, uint8_t num, void *addr)
{
    while (!BIT_FLAG(inb(0x1f7), 6)) // while not ready
        __asm__("nop");   // wait
    outb(0x1f2, num); /* read num sectors */
    outb(0x1f3, sect); /* sector number */
    outb(0x1f4, cyl & 0xff);
    outb(0x1f5, cyl >> 8);
    outb(0x1f6, head | 0xa0);
    outb(0x1F7, 0x20);                      // cmd 0x20 - read sectors
    while (BIT_FLAG(inb(0x1f7), 7)) // while busy
        __asm__("nop");   // wait
    insl(0x1f0, addr, 128 * num);
}

#define elf_head ((struct elfhdr *) 0x10000)
#define buf ((void *) 0x80000)

void bootmain()
{
    struct proghdr *pharr;

    read_sector(2, 0, 0, 1, (void *) elf_head);
    if (elf_head->e_magic != ELF_MAGIC) {
        __asm__("cli; hlt");
    }

    pharr = (struct proghdr *)((uint32_t)elf_head + elf_head->e_phoff);
    for (int i = 0; i < elf_head->e_phnum; i++) {
        /* sector index starts at 1 instead of 0 */
        read_sector(pharr[i].p_offset / 512 + 2, 0, 0, pharr[i].p_filesz / 512 + 1, buf);
        memcpy((void *) pharr[i].p_va, buf + (pharr[i].p_offset % 512), pharr[i].p_filesz);
    }

    ((void (*)()) elf_head->e_entry)();
}
