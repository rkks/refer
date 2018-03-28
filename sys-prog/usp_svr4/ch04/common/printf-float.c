#include <stdio.h>

#define N   4

double numbers[N] = { 0, -1.234, 67.890, 1234567.98765 };

int
main(int argc, char **argv)
{
    int i;

    for (i = 0; i < N; i++) {
        printf("f notation: %f\n", numbers[i]);
        printf("e notation: %e\n", numbers[i]);
        printf("g notation: %g\n\n", numbers[i]);
    }

    exit(0);
}
