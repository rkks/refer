#include <string.h>

void    markLine(char *, char *, char *);
void    outputLine(char *);
char    *inputLine(void);

int
main(int argc, char **argv)
{
    char copyline[1024];
    char *p, *line, *token, *fieldsep;

    if (argc != 3) {
        outputLine("Usage: search-token token fieldsep");
        exit(1);
    }

    token = argv[1];
    fieldsep = argv[2];

    /*
     * For each line in the input...
     */
    while ((line = inputLine()) != NULL) {
        /*
         * Save a copy of the line.
         */
        strcpy(copyline, line);

        /*
         * Find the first token.
         */
        if ((p = strtok(line, fieldsep)) == NULL)
            continue;

        /*
         * Search through all the tokens.
         */
        do {
            if (strcmp(p, token) == 0) {
                outputLine(copyline);
                markLine(copyline, copyline + (p - line),
                         copyline + (p - line) + strlen(token) - 1);
                outputLine(copyline);
                p = NULL;
            }
            else {
                p = strtok(NULL, fieldsep);
            }
        } while (p != NULL);
    }

    exit(0);
}
