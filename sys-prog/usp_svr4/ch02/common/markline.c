#include <stdio.h>

void
markLine(char *line, char *start, char *stop)
{
    char *p;

    for (p = line; p < start; p++)
        *p = ' ';

    for (p = start; p <= stop; p++)
        *p = '^';

    for (p = stop+1; *p != '\0'; p++)
        *p = ' ';
}
