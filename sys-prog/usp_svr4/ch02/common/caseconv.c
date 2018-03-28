#include <ctype.h>

void    outputChar(char);
int     inputChar(void);

int
main(int argc, char **argv)
{
    int c;

    while ((c = inputChar()) >= 0) {
        if (isupper(c))
            outputChar(tolower(c));
        else if (islower(c))
            outputChar(toupper(c));
        else
            outputChar(c);
    }

    exit(0);
}
