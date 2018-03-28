#include <unistd.h>
#include <libgen.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
    FILE *fp;
    char line[BUFSIZ];
    char *pattern, *filename;

    /*
     * Check arguments.
     */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s pattern file\n", *argv);
        exit(1);
    }

    printf("NOTE: HP-UX 10.x does not provide the gmatch() function.\n");
    printf("      This example uses fnmatch() to emulate gmatch().\n\n");

    pattern = *++argv;
    filename = *++argv;

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
        if (gmatch(line, pattern) != 0)
            puts(line);
    }

    fclose(fp);
    exit(0);
}

int
gmatch(const char *str, const char *pattern)
{
    return(!fnmatch(pattern, str, 0));
}
