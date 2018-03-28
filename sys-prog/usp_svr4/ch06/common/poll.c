#include <stropts.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>

#define NTTYS       3           /* number of ttys to use     */
#define TIMEOUT     10          /* number of seconds to wait */

int     fds[NTTYS];             /* file descriptors          */
char    *fileNames[NTTYS];      /* file names                */

int     openFiles(char **);
void    readFiles(struct pollfd *);

int
main(int argc, char **argv)
{
    int i, n, maxfd;
    struct pollfd pfds[NTTYS];

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
     * We only need to initialize these once.
     */
    for (i=0; i < NTTYS; i++) {
        pfds[i].fd = fds[i];
        pfds[i].events = POLLIN;
    }

    /*
     * Forever...
     */
    for (;;) {
        /*
         * Wait for some input.
         */
        n = poll(pfds, NTTYS, TIMEOUT * 1000);

        /*
         * See what happened.
         */
        switch (n) {
        case -1:            /* error           */
            perror("poll");
            exit(1);
        case 0:             /* timeout         */
            printf("\nTimeout expired.  Type something!\n");
            break;
        default:            /* input available */
            readFiles(pfds);
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
readFiles(struct pollfd *pfds)
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
        if (pfds[i].revents & POLLIN) {
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
