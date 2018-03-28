#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>

#define NTTYS       3           /* number of ttys to use     */
#define TIMEOUT     10          /* number of seconds to wait */

int     fds[NTTYS];             /* file descriptors          */
char    *fileNames[NTTYS];      /* file names                */

int     openFiles(char **);
void    readFiles(fd_set *);

int
main(int argc, char **argv)
{
    fd_set readfds;
    int i, n, maxfd;
    struct timeval tv;

    /*
     * Check that we have the right number of arguments.
     */
    if (argc != (NTTYS+1)) {
        fprintf(stderr, "You must supply %d tty names.\n", NTTYS);
        exit(1);
    }

    /*
     * Open the files.  The highest numbered file descriptor
     * (plus one) is returned in maxfd.
     */
    maxfd = openFiles(++argv);

    /*
     * Forever...
     */
    for (;;) {
        /*
         * Zero the bitmask.
         */
        FD_ZERO(&readfds);

        /*
         * Set bits in the bitmask.
         */
        for (i=0; i < NTTYS; i++)
            FD_SET(fds[i], &readfds);

        /*
         * Set up the timeout.
         */
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;

        /*
         * Wait for some input.
         */
        n = select(maxfd, (int *) &readfds, (int *) 0, (int *) 0, &tv);

        /*
         * See what happened.
         */
        switch (n) {
        case -1:            /* error           */
            perror("select");
            exit(1);
        case 0:             /* timeout         */
            printf("\nTimeout expired.  Type something!\n");
            break;
        default:            /* input available */
            readFiles(&readfds);
            break;
        }
    }
}

/*
 * openFiles - open all the files, return the highest file descriptor.
 */
int
openFiles(char **files)
{
    int i, maxfd;

    maxfd = 0;

    /*
     * For each file...
     */
    for (i=0; i < NTTYS; i++) {
        /*
         * Open it.
         */
        if ((fds[i] = open(*files, O_RDONLY)) < 0) {
            perror(*files);
            exit(1);
        }

        /*
         * Make sure it's a tty.
         */
        if (!isatty(fds[i])) {
            fprintf(stderr, "All files must be tty devices.\n");
            exit(1);
        }

        /*
         * Save the name.
         */
        fileNames[i] = *files++;

        /*
         * Save the highest numbered fd.
         */
        if (fds[i] > maxfd)
            maxfd = fds[i];
    }

    return(maxfd + 1);
}

/*
 * readFiles - read input from any files that have some.
 */
void
readFiles(fd_set *readfds)
{
    int i, n;
    char buf[BUFSIZ];

    /*
     * For each file...
     */
    for (i=0; i < NTTYS; i++) {
        /*
         * If it has some input available...
         */
        if (FD_ISSET(fds[i], readfds)) {
            /*
             * Read the data.
             */
            n = read(fds[i], buf, sizeof(buf));
            buf[n] = '\0';

            /*
             * Print it out.
             */
            printf("\nRead %d bytes from %s:\n", n, fileNames[i]);
            printf("\t%s\n", buf);

            /*
             * Is it telling us to stop?
             */
            if (strcmp(buf, "S-T-O-P\n") == 0)
                exit(0);
        }
    }
}
