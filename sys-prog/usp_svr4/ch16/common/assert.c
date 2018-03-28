#include <assert.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
    int total;

    total = 0;

    while (--argc)
        total += atoi(*++argv);

    assert(total > 100);

    printf("%d\n", total);
    exit(0);
}
