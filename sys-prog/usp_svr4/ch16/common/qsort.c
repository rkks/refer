#include <stdlib.h>

#define NELEM   10

int compare(const void *, const void *);

int
main(void)
{
    int i;
    int array[NELEM];

    /*
     * Fill the array with numbers.
     */
    for (i = 0; i < NELEM; i++)
        array[NELEM - i - 1] = (i * i) & 0xf;

    /*
     * Print it.
     */
    printf("Before sorting:\n\t");

    for (i = 0; i < NELEM; i++)
        printf("%d ", array[i]);
    putchar('\n');

    /*
     * Sort it.
     */
    qsort(array, NELEM, sizeof(int), compare);

    /*
     * Print it again.
     */
    printf("After sorting:\n\t");

    for (i = 0; i < NELEM; i++)
        printf("%d ", array[i]);
    putchar('\n');

    exit(0);
}

/*
 * compare - compare two integers.
 */
int
compare(const void *a, const void *b)
{
    int *aa, *bb;

    aa = (int *) a;
    bb = (int *) b;

    return(*aa - *bb);
}
