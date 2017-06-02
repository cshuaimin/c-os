#include "asmhelp.h"

void
write_string(int color, const char *string)
{
    volatile char *video = (volatile char *)0xb8000;
    while(*string) {
        *video++ = *string++;
        *video++ = color;
    }
}

void
kernel_main()
{
    write_string(321, "hello, world!");
    volatile char *video = (volatile char *)0xb8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video[i] = 'A' + (i % 26);
        video[i+1] = i & 0x0f;
    }
}
