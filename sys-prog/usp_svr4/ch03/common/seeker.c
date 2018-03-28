#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define NSTRINGS    5
#define STRSIZE     3

char *strings[] = {
    "aaa", "bbb", "ccc", "ddd", "eee"
};

int
main(int argc, char **argv)
{
    int n, fd;
    char *fname;
    char buf[STRSIZE], answer[8], template[32];

    /*
     * Create a temporary file name.
     */
    strcpy(template, "/tmp/seekerXXXXXX");
    fname = mktemp(template);

    /*
     * Create the file.
     */
    if ((fd = open(fname, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0) {
        perror(fname);
        exit(1);
    }

    /*
     * Write strings to the file.
     */
    for (n = 0; n < NSTRINGS; n++)
        write(fd, strings[n], STRSIZE);

    /*
     * Until the user quits, prompt for a string and retrieve
     * it from the file.
     */
    for (;;) {
        /*
         * Prompt for the string number.
         */
        write(1, "Which string (0 to quit)? ", 26);
        n = read(0, answer, sizeof(answer));
        answer[n-1] = '\0';
        n = atoi(answer);

        if (n == 0) {
            close(fd);
            exit(0);
        }

        if (n < 0 || n > NSTRINGS) {
            write(2, "Out of range.\n", 14);
            continue;
        }

        /*
         * Find the string and read it.
         */
        lseek(fd, (n-1) * STRSIZE, SEEK_SET);
        read(fd, buf, STRSIZE);

        /*
         * Print it out.
         */
        write(1, "String ", 7);
        write(1, answer, strlen(answer));
        write(1, " = ", 3);
        write(1, buf, STRSIZE);
        write(1, "\n\n", 2);
    }
}
