#include <locale.h>
#include <stdio.h>
#include <time.h>

/*
 * Sample formats.
 */
char *formats[] = {
    "%A, %B %e, %Y, %H:%M:%S",
    "%I:%M %p, %d-%b-%y",
    "%x %X",
    "%C",
    "%c",
    NULL
};

char *locales[] = {
    "C", "de", "fr", "it", "sv", NULL
};

char *localeNames[] = {
    "UNIX", "German", "French", "Italian", "Swedish", NULL
};

int
main(int argc, char **argv)
{
    int i, j;
    time_t clock;
    struct tm *tm;
    char buf[BUFSIZ];

    /*
     * Get current time.
     */
    time(&clock);
    tm = gmtime(&clock);

    /*
     * For each locale...
     */
    for (i=0; locales[i] != NULL; i++) {
        /*
         * Print the locale name and set it.
         */
        printf("%s:\n", localeNames[i]);
        setlocale(LC_TIME, locales[i]);

        /*
         * For each format string...
         */
        for (j=0; formats[j] != NULL; j++) {
            strftime(buf, sizeof(buf), formats[j], tm);
            printf("\t%-25s %s\n", formats[j], buf);
        }

        printf("\n");
    }

    exit(0);
}
