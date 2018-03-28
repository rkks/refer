#include <stdio.h>

#define COST_PER_ITEM   1.25

void    printCost(int);

int
main(int argc, char **argv)
{
    int i;

    for (i = 1; i < 1000; i *= 10)
        printCost(i);

    exit(0);
}

void
printCost(int n)
{
    printf("Cost of %3d items at $%.2f each = $%6.2f\n", n, COST_PER_ITEM,
           n * COST_PER_ITEM);
}
