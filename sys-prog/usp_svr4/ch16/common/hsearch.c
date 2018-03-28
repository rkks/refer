#include <search.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct data {
    int age;
    int height;
    int weight;
};

int
main(void)
{
    char *p;
    ENTRY item;
    ENTRY *result;
    struct data *d;
    char buf[BUFSIZ];
    
    /*
     * Create the hash table.
     */
    hcreate(100);

    printf("Enter Name/age/height/weight; terminate with blank line.\n\n");

    /*
     * Read information until a blank line.
     */
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        /*
         * Blank line, all done.
         */
        if (*buf == '\n')
            break;
        
        /*
         * Allocate a data structure (we should check for
         * errors here).
         */
        d = (struct data *) malloc(sizeof(struct data));
        item.data = (char *) d;

        /*
         * Split up the data (we should check for errors
         * here).
         */
        p = strtok(buf, "/");
        item.key = strdup(p);

        p = strtok(NULL, "/");
        d->age = atoi(p);

        p = strtok(NULL, "/");
        d->height = atoi(p);

        p = strtok(NULL, "/");
        d->weight = atoi(p);

        /*
         * Add the item to the table.
         */
        (void) hsearch(item, ENTER);
    }

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
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            putchar('\n');
            hdestroy();
            exit(0);
        }

        /*
         * Strip the newline.
         */
        buf[strlen(buf) - 1] = '\0';

        /*
         * Look in the table for the item.
         */
        item.key = buf;
        result = hsearch(item, FIND);

        /*
         * Print the search results.
         */
        if (result == NULL) {
            printf("Entry not found.\n");
        }
        else {
            d = (struct data *) result->data;
            printf("Name: %s\nAge: %d\nHeight: %d\nWeight: %d\n",
                   result->key, d->age, d->height, d->weight);
        }
    }
}
