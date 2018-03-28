#include <search.h>
#include <string.h>
#include <stdio.h>

#define TABLESIZE   10      /* max. size of the table       */
#define ELEMENTSIZE 16      /* max. size of a table element */

int compare(const void *, const void *);

int
main(void)
{
    int i;
    char *p;
    size_t nel;
    char line[ELEMENTSIZE];
    char table[TABLESIZE][ELEMENTSIZE];

    /*
     * Tell the user what to do.
     */
    printf("Enter %d strings, not all unique.\n\n", TABLESIZE);

    /*
     * Read in some strings.
     */
    nel = 0;
    for (i = 0; i < TABLESIZE; i++) {
        /*
         * Prompt for each string.
         */
        printf("%2d> ", i + 1);

        /*
         * Read the string.
         */
        if (fgets(line, sizeof(line), stdin) == NULL)
            exit(0);

        /*
         * Strip the newline.
         */
        line[strlen(line) - 1] = '\0';

        /*
         * Search for the string.  If it's not in the table,
         * lsearch will add it for us.
         */
        (void) lsearch(line, table, &nel, ELEMENTSIZE, compare);
    }

    /*
     * Print the contents of the table.
     */
    printf("\nContents of the table:\n");

    for (i = 0; i < nel; i++)
        printf("\t%s\n", table[i]);

    /*
     * Let the user search for things.
     */
    for (;;) {
        /*
         * Prompt for a search string.
         */
        printf("\nSearch for: ");

        /*
         * Read the search string.
         */
        if (fgets(line, sizeof(line), stdin) == NULL) {
            putchar('\n');
            exit(0);
        }

        /*
         * Strip the newline.
         */
        line[strlen(line) - 1] = '\0';

        /*
         * Search for the string.  lfind will return null
         * if it's not there.
         */
        p = (char *) lfind(line, table, &nel, ELEMENTSIZE, compare);

        /*
         * Print the search results.
         */
        if (p == NULL) {
            printf("String not found.\n");
        }
        else {
            printf("Found at location %d.\n",
                   ((int) p - (int) table) / ELEMENTSIZE + 1);
        }
    }
}

/*
 * compare - compare two strings, return 0 if equal, non-zero if not.
 */
int
compare(const void *a, const void *b)
{
    return(strcmp((char *) a, (char *) b));
}
