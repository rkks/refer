#include <libgen.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
    FILE *fp;
    char line[BUFSIZ];
    char *re, *pattern, *filename;

    /*
     * Check arguments.
     */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s pattern file\n", *argv);
        exit(1);
    }

    pattern = *++argv;
    filename = *++argv;

    /*
     * Compile the regular expression.
     */
    if ((re = regcmp(pattern, NULL)) == NULL) {
        fprintf(stderr, "bad regular expression.\n");
        exit(1);
    }

    /*
     * Open the file.
     */
    if ((fp = fopen(filename, "r")) == NULL) {
        perror(filename);
        exit(1);
    }

    /*
     * Read lines from the file.
     */
    while (fgets(line, sizeof(line), fp) != NULL) {
        /*
         * Strip the newline.
         */
        line[strlen(line) - 1] = '\0';

        /*
         * If it matches, print it.
         */
        if (regex(re, line) != NULL)
            puts(line);
    }

    fclose(fp);
    exit(0);
}
