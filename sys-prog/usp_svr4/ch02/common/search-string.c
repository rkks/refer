#include <string.h>

void    markLine(char *, char *, char *);
void    outputLine(char *);
char    *inputLine(void);

int
main(int argc, char **argv)
{
    char *p, *line, *string;

    if (argc != 2) {
        outputLine("Usage: search-string string");
        exit(1);
    }

    string = argv[1];

    while ((line = inputLine()) != NULL) {
        if ((p = strstr(line, string)) != NULL) {
            outputLine(line);
            markLine(line, p, p + strlen(string) - 1);
            outputLine(line);
        }
    }

    exit(0);
}
