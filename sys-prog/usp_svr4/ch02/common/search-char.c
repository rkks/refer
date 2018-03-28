#include <string.h>

void    markLine(char *, char *, char *);
void    outputLine(char *);
char    *inputLine(void);

int
main(int argc, char **argv)
{
    char c;
    char *p, *line;

    if (argc != 2) {
        outputLine("Usage: search-char character");
        exit(1);
    }

    c = argv[1][0];

    while ((line = inputLine()) != NULL) {
        if ((p = strchr(line, c)) != NULL) {
            outputLine(line);
            markLine(line, p, p);
            outputLine(line);
        }
    }

    exit(0);
}
