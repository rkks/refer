/* Gregory Pietsch <gkp1@flash.net> */

/*
 * Here's my attempt at a Category 0 version of 1-10.
 *
 * Gregory Pietsch
 */

#include <stdio.h>

int main()
{
    int c, d;

    while ( (c=getchar()) != EOF) {
        d = 0;
        if (c == '\\') {
            putchar('\\');
            putchar('\\');
            d = 1;
        }
        if (c == '\t') {
            putchar('\\');
            putchar('t');
            d = 1;
        }
        if (c == '\b') {
            putchar('\\');
            putchar('b');
            d = 1;
        }
        if (d == 0)
            putchar(c);        
    }
    return 0;
}

