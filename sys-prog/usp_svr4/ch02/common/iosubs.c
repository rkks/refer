#include <stdio.h>

/*
 * inputChar - read and return one character from standard input.
 */
int
inputChar(void)
{
    return(getc(stdin));
}

/*
 * inputLine - read and return one line from standard input.
 */
char *
inputLine(void)
{
    int c, n;
    static char line[BUFSIZ+1];

    n = 0;
    while ((c = getc(stdin)) != '\n') {
        if (c == EOF)
            return(NULL);

        if (n < BUFSIZ)
            line[n++] = c;
    }

    line[n] = '\0';
    return(line);
}

/*
 * outputChar - print one character on standard output.
 */
void
outputChar(char c)
{
    putc(c, stdout);
}

/*
 * outputLine - output one line on standard output.
 */
void
outputLine(char *line)
{
    char *p;

    for (p = line; *p != '\0'; p++)
        putc(*p, stdout);

    putc('\n', stdout);
}
