#include <string.h>
#include <libgen.h>

void    outputLine(char *);
char    *inputLine(void);

char *strtrns(const char *, const char *, const char *, char *);

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

    outputLine("NOTE: HP-UX 10.x does not provide the strtrns() function.");
    outputLine("      This example uses its own version of strtrns().");
    outputLine("");

    while ((line = inputLine()) != NULL) {
        p = strtrns(line, old, new, newline);
        outputLine(p);
    }

    exit(0);
}

char *
strtrns(const char *string, const char *old, const char *new, char *result)
{
    char *resp;
    const char *oldp;

    for (resp = result; *resp = *string++; resp++) {
        for (oldp = old; *oldp != '\0'; ) {
            if (*resp == *oldp++) {
                *resp = new[oldp - old - 1];
                break;
            }
        }
    }

    return(result);
}
