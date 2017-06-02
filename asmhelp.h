#include "defs.h"

uint8_t __always_inline
inb(uint16_t port)
{
    uint8_t data;
    __asm__ __volatile__ (
            "inb %1, %0"
            : "=a" (data)
            /* If the port number is in the range of 0-255
               it can be specified as an immediate, otherwise
               the port number must be specified in DX. */
            : "d" (port));
    return data;
}

void __always_inline
outb(uint16_t port, uint8_t data)
{
    __asm__ __volatile__ (
            "outb %0, %1"
            :
            : "a" (data), "d" (port));
}

void __always_inline
insl(uint16_t port, void *addr, uint32_t cnt)
{
    __asm__ __volatile__ (
            "cld;"
            "repne; insl;"
            : "=D" (addr), "=c" (cnt)
            : "d" (port), "0" (addr), "1" (cnt)
            : "memory", "cc");
}

void *
memcpy(void *dst, const void *src, uint32_t n)
{
    char *d = dst;
    const char *s = src;
    while (n--)
        *d++ = *s++;
    return dst;
}
