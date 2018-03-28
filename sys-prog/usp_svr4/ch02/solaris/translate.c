#include <string.h>
#include <libgen.h>

void    outputLine(char *);
char    *inputLine(void);

int
main(int argc, char **argv)
{
    char newline[1024];
    char *p, *old, *new, *line;

    if (argc != 3) {
        outputLine("Usage: translate old new");
        exit(1);
    }

    old = argv[1];
    new = argv[2];

    if (strlen(old) != strlen(new)) {
        outputLine("old and new strings must be same length.");
        exit(1);
    }

    while ((line = inputLine()) != NULL) {
        p = strtrns(line, old, new, newline);
        outputLine(p);
    }

    exit(0);
}
