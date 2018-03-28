#include <string.h>

#define NSTRINGS    16          /* max. number of strings       */
#define MAXLENGTH   1024        /* max. length of one string    */

void    bubbleSort(char **, int);
void    outputLine(char *);
char    *inputLine(void);

int
main(int argc, char **argv)
{
    int n, nstrings;
    char *p, *q, *line;
    char *strptrs[NSTRINGS];
    char strings[NSTRINGS][MAXLENGTH];

    /*
     * Read in NSTRINGS strings from the standard input.
     */
    for (nstrings = 0; nstrings < NSTRINGS; nstrings++) {
        /*
         * Get a line from the input.
         */
        if ((line = inputLine()) == NULL)
            break;

        /*
         * Copy the line.
         */
        for (p = line, q = strings[nstrings]; *p != '\0'; p++, q++)
            *q = *p;
        *q = '\0';

        /*
         * Save a pointer to the line.
         */
        strptrs[nstrings] = strings[nstrings];
    }

    /*
     * Sort the strings.
     */
    bubbleSort(strptrs, nstrings);

    /*
     * Print the strings.
     */
    for (n = 0; n < nstrings; n++)
        outputLine(strptrs[n]);

    exit(0);
}

/*
 * bubbleSort - implementation of the basic bubble sort algorithm.
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
             * Use strlen() to compare the strings
             * by length.
             */
            if (strlen(strings[i]) > strlen(strings[i+1])) {
                tmp = strings[i+1];
                strings[i+1] = strings[i];
                strings[i] = tmp;
                notdone = 1;
            }
        }
    }
}
