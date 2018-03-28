#include <stdio.h>

/* count lines in input */
int
main()
{
        int c, pc; /* c = character, pc = previous character */

        /* set pc to a value that wouldn't match any character, in case
        this program is ever modified to get rid of multiples of other
        characters */

        pc = EOF;

        while ((c = getchar()) != EOF) {
                if (c == ' ')
                        if (pc != ' ')   /* or if (pc != c) */ 
                                putchar(c);

                /* We haven't met 'else' yet, so we have to be a little clumsy */
                if (c != ' ')
                        putchar(c);
                pc = c;
        }

        return 0;
}


