#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <ndbm.h>

struct data {
    int age;
    int height;
    int weight;
};

int
main(void)
{
    char *p;
    DBM *db;
    struct data d;
    char buf[BUFSIZ];
    datum key, content;
    
    /*
     * Create the database.
     */
    if ((db = dbm_open("people", O_RDWR|O_CREAT, 0666)) == NULL) {
        perror("people");
        exit(1);
    }
    
    printf("Enter Name/age/height/wight; terminate with blank line.\n\n");

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
         * The content will be the data structure.
         */
        content.dptr = (char *) &d;
        content.dsize = sizeof(struct data);
        
        /*
         * Split up the data (we should check for errors
         * here).
         */
        p = strtok(buf, "/");
        key.dptr = strdup(p);
        key.dsize = strlen(p);

        p = strtok(NULL, "/");
        d.age = atoi(p);

        p = strtok(NULL, "/");
        d.height = atoi(p);

        p = strtok(NULL, "/");
        d.weight = atoi(p);

        /*
         * Add the item to the database.
         */
        (void) dbm_store(db, key, content, DBM_INSERT);
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
            dbm_close(db);
            putchar('\n');
            exit(0);
        }

        /*
         * Strip the newline.
         */
        buf[strlen(buf) - 1] = '\0';

        /*
         * Look in the database for the item.
         */
        key.dptr = buf;
        key.dsize = strlen(buf);

        content = dbm_fetch(db, key);

        /*
         * Print the search results.
         */
        if (content.dptr == NULL) {
            printf("Entry not found.\n");
        }
        else {
            /*
             * Copy the structure to insure it is aligned
             * properly.
             */
            memcpy((char *) &d, content.dptr, content.dsize);

            printf("Name: %s\nAge: %d\nHeight: %d\nWeight: %d\n",
                   buf, d.age, d.height, d.weight);
        }
    }
}
