/* 1-21.c */

#include <stdio.h>

#define TABSTOP 4

int main(void)
{
    size_t spaces = 0;
    int ch;
    size_t x = 0;               /* position in the line */
    size_t tabstop = TABSTOP;   /* get this from the command-line 
                                 * if you want to */

    while ((ch = getchar()) != EOF)
    {
        if (ch == ' ')
        {
            spaces++;
        }
        else if (spaces == 0) /* no space, just printing */
        {
            putchar(ch);
            x++;
        }
        else if (spaces == 1) /* just one space, never print a tab */
        {
            putchar(' ');
            putchar(ch);
            x += 2;
            spaces = 0;
        }
        else
        {
            while (x / tabstop != (x + spaces) / tabstop) 
                /* are the spaces reaching behind the next tabstop ? */
            {
                putchar('\t');
                x++;
                spaces--;
                while (x % tabstop != 0)
                {
                    x++;
                    spaces--;
                }
            }

            while (spaces > 0) /* the remaining ones are real space */
            {
                putchar(' ');
                x++;
                spaces--;
            }
            putchar(ch); /* now print the non-space char */
            x++;
        }
        if (ch == '\n')
        {
            x = 0; /* reset line position */
        }
    }

    return 0;
}
