#include <stdio.h>

#define N   4

int numbers[N] = { 0, -1, 3, 169 };

int
main(int argc, char **argv)
{
    int i;

    for (i = 0; i < N; i++) {
        printf("Signed decimal:       %d\n", numbers[i]);
        printf("Unsigned octal:       %o\n", numbers[i]);
        printf("Unsigned decimal:     %u\n", numbers[i]);
        printf("Unsigned hexadecimal: %x\n\n", numbers[i]);
    }

    exit(0);
}
