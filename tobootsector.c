#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

long get_file_len(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return len;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s binary-input bootsector-output\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char buf[512] = {0};
    FILE *fp = fopen(argv[1], "rb");
    assert(fp);
    long len = get_file_len(fp);
    if (len > 510) {
        fprintf(stderr, "Bootsector is bigger than 510 bytes(%ld)!\n", len);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fread(buf, 510, 1, fp);
    fclose(fp);
    buf[510] = 0x55;
    buf[511] = 0xaa;
    fp = fopen(argv[2], "wb");
    assert(fp);
    fwrite(buf, 512, 1, fp);
    fclose(fp);
    return 0;
}
