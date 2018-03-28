#include <stdio.h>

int
main(int argc, char **argv)
{
    int n;
    FILE *in, *out;
    char buf[BUFSIZ];

    if (argc != 3) {
        fprintf(stderr, "Usage: append-line file1 file2\n");
        exit(1);
    }

    /*
     * Open the first file for reading.
     */
    if ((in = fopen(argv[1], "r")) == NULL) {
        perror(argv[1]);
        exit(1);
    }

    /*
     * Open the second file for writing.
     */
    if ((out = fopen(argv[2], "a")) == NULL) {
        perror(argv[2]);
        exit(1);
    }

    /*
     * Copy data from the first file to the second, a buffer
     * full at a time.
     */
    while ((n = fread(buf, sizeof(char), BUFSIZ, in)) > 0)
        fwrite(buf, sizeof(char), n, out);

    fclose(out);
    fclose(in);
    exit(0);
}
