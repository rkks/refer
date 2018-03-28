#include <search.h>
#include <string.h>
#include <stdio.h>

#define TABLESIZE   32768       /* max. size of the table       */
#define ELEMENTSIZE 32          /* max. size of a table element */

int compare(const void *, const void *);

int
main(void)
{
    char *p;
    FILE *fp;
    size_t nel;
    char line[ELEMENTSIZE];
    char table[TABLESIZE][ELEMENTSIZE];

    /*
     * Open the file.
     */
    if ((fp = fopen("/usr/dict/words", "r")) == NULL) {
        perror("/usr/dict/words");
        exit(1);
    }

    printf("Reading the table... ");
    fflush(stdout);

    /*
     * Read in the file.
     */
    for (nel = 0; nel < TABLESIZE; nel++) {
        /*
         * Read a line.
         */
        if (fgets(table[nel], ELEMENTSIZE, fp) == NULL)
            break;

        /*
         * Strip the newline.
         */
        table[nel][strlen(table[nel]) - 1] = '\0';
    }
    
    printf("done.\n");
    fclose(fp);
    
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
         * Do a binary search for the string.
         */
        p = (char *) bsearch(line, table, nel, ELEMENTSIZE, compare);

        /*
         * Print the search results.
         */
        if (p == NULL) {
            printf("String not found.\n");
        }
        else {
            printf("Found at location %d.\n",
                   ((int) p - (int) table) / ELEMENTSIZE);
        }
    }
}

/*
 * compare - compare two strings, return 0 if equal, non-zero if not.
 */
int
compare(const void *a, const void *b)
{
    return(strcasecmp((char *) a, (char *) b));
}
