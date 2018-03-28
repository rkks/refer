#include <string.h>

void    markLine(char *, char *, char *);
void    outputLine(char *);
char    *inputLine(void);

int
main(int argc, char **argv)
{
    char *p, *line, *charset;

    if (argc != 2) {
        outputLine("Usage: search-charset character-set");
        exit(1);
    }

    charset = argv[1];

    while ((line = inputLine()) != NULL) {
        if ((p = strpbrk(line, charset)) != NULL) {
            outputLine(line);
            markLine(line, p, p);
            outputLine(line);
        }
    }

    exit(0);
}
