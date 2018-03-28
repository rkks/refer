#include <stdio.h>

int
main(int argc, char **argv)
{
    double total;
    int n, quarters, dimes, nickels;

    for (;;) {
        printf("Enter a line like:\n");
        printf("%%d quarters, %%d dimes, %%d nickels\n");
        printf("--> ");

        n = scanf("%d quarters, %d dimes, %d nickels", &quarters, &dimes,
                  &nickels);

        if (n != 3)
            exit(0);

        total = quarters * 0.25 + dimes * 0.10 + nickels * 0.05;

        printf("You have: $ %.2f\n\n", total);
    }
}
