#include <stdio.h>
#include <time.h>

extern int getdate_err;

int
main(int argc, char **argv)
{
    struct tm *tm;
    char buf[BUFSIZ];

    for (;;) {
        /*
         * Prompt for a string.
         */
        printf("? ");

        /* 
         * Read the string.
         */
        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            putchar('\n');
            exit(0);
        }

        /*
         * Convert it.
         */
        if ((tm = getdate(buf)) != NULL)
            printf("%s\n", asctime(tm));
        else
            printf("Error (%d).\n", getdate_err);
    }
}
