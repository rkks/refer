#include <stdlib.h>
#include <string.h>

void    bubbleSort(char **, int);
void    outputLine(char *);
char    *inputLine(void);

int
main(int argc, char **argv)
{
    char *line;
    char **strptrs = NULL;
    int n, nstrings, nstrptrs;

    nstrings = 0;
    nstrptrs = 0;

    /*
     * For each line in the input...
     */
    while ((line = inputLine()) != NULL) {
        /*
         * If we're full up, allocate some more pointers.
         */
        if (nstrings == nstrptrs) {
            if (nstrptrs == 0) {
                nstrptrs = 8;
                strptrs = malloc(nstrptrs * sizeof(char *));
            }
            else {
                nstrptrs += 8;
                strptrs = realloc(strptrs, nstrptrs * sizeof(char *));
            }

            if (strptrs == NULL) {
                outputLine("out of memory.");
                exit(1);
            }
        }

        /*
         * Save a pointer to the line, stored in dynamically
         * allocated memory.
         */
        strptrs[nstrings++] = strdup(line);
    }

    /*
     * Sort the strings.
     */
    bubbleSort(strptrs, nstrings);

    /*
     * Print the strings and free the memory.
     */
    for (n = 0; n < nstrings; n++) {
        outputLine(strptrs[n]);
        free(strptrs[n]);
    }

    free(strptrs);
    exit(0);
}

/*
 * bubbleSort - implementation of the standard bubble sort algorithm.
 */
void
bubbleSort(char **strings, int nstrings)
{
    int i, j;
    char *tmp;
    int notdone;

    j = nstrings;
    notdone = 1;

    while (notdone) {
        notdone = 0;
        j = j - 1;

        for (i = 0; i < j; i++) {
            /*
             * Use strcmp() to compare the strings
             * alphabetically.
             */
            if (strcmp(strings[i], strings[i+1]) > 0) {
                tmp = strings[i+1];
                strings[i+1] = strings[i];
                strings[i] = tmp;
                notdone = 1;
            }
        }
    }
}
