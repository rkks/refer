#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define NSTRINGS    5
#define STRSIZE     3

char *strings[] = {
    "aaa", "bbb", "ccc", "ddd", "eee"
};

int
main(int argc, char **argv)
{
    int n;
    FILE *fp;
    char *fname;
    char buf[STRSIZE], template[32];

    /*
     * Create a temporary file name.
     */
    strcpy(template, "/tmp/seekerXXXXXX");
    fname = mktemp(template);

    /*
     * Open the file.
     */
    if ((fp = fopen(fname, "w+")) == NULL) {
        perror(fname);
        exit(1);
    }

    /*
     * Write strings to the file.
     */
    for (n = 0; n < NSTRINGS; n++)
        fwrite(strings[n], sizeof(char), STRSIZE, fp);

    /*
     * Until the user quits, prompt for a string and retrieve
     * it from the file.
     */
    for (;;) {
        /*
         * Prompt for a string number.
         */
        printf("Which string (0 to quit)? ");
        scanf("%d", &n);

        if (n == 0) {
            fclose(fp);
            exit(0);
        }

        if (n < 0 || n > NSTRINGS) {
            fprintf(stderr, "Out of range.\n");
            continue;
        }

        /*
         * Find the string and read it.
         */
        fseek(fp, (n-1) * STRSIZE, SEEK_SET);
        fread(buf, sizeof(char), STRSIZE, fp);

        /*
         * Print it out.
         */
        printf("String %d = %.*s\n\n", n, STRSIZE, buf);
    }
}
