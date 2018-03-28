#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <stropts.h>
#include <termios.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

/* HP-UX forgets to define this */
char *ptsname(int);

#define MAXARGS 32                          /* max. cmd. args     */

char        *shell = "/bin/sh";             /* default shell      */
char        *filename = "scriptfile";       /* default file       */
char        *mastername = "/dev/ptmx";      /* pty clone device   */

int     master;                             /* master side of pty */
FILE        *script;                        /* script file        */
struct termios  newtty, origtty;            /* tty modes          */

void    finish(int);
int ptyopen(char *, struct termios *);

int
main(int argc, char **argv)
{
    char *p;
    int n, nfd;
    time_t clock;
    fd_set readmask;
    char buf[BUFSIZ];
    struct sigaction sact;

    /*
     * If an argument is given, it's a new script file.
     */
    if (argc > 1)
        filename = *++argv;

    /*
     * 1. Use the user's shell, if known.
     */
    if ((p = getenv("SHELL")) != NULL)
        shell = p;

    /*
     * 2. Open the script file.
     */
    if ((script = fopen(filename, "w")) == NULL) {
        perror(filename);
        exit(1);
    }

    /*
     * 3. Get the tty modes.  We'll use these both to
     *    set modes on the pseudo-tty, and to restore
     *    modes on the user's tty.
     */
    if (tcgetattr(0, &origtty) < 0) {
        perror("tcgetattr: stdin");
        exit(1);
    }

    /*
     * 4. Grab a pseudo-tty and start a shell on it.
     */
    if ((master = ptyopen(shell, &origtty)) < 0)
        exit(1);

    /*
     * Print a little start message.
     */
    time(&clock);
    fprintf(script, "Script started on %s", ctime(&clock));
    printf("Script started, file is %s\n", filename);

    /*
     * 5. We need to catch signals, now that we're going
     *    to change tty modes.
     */
    memset((char *) &sact, 0, sizeof(struct sigaction));
    sact.sa_handler = finish;
    sigaction(SIGINT, &sact, (struct sigaction *) 0);
    sigaction(SIGQUIT, &sact, (struct sigaction *) 0);

    /*
     * 6. Change the user's tty modes such that pretty
     *    much everything gets passed through to the
     *    pseudo-tty.  Set "raw" mode so that we can pass
     *    characters as they're typed, etc.
     */
    newtty = origtty;
    newtty.c_cc[VMIN] = 1;
    newtty.c_cc[VTIME] = 0;
    newtty.c_oflag &= ~OPOST;
    newtty.c_lflag &= ~(ICANON|ISIG|ECHO);
    newtty.c_iflag &= ~(INLCR|IGNCR|ICRNL|IUCLC|IXON);

    /*
     * 7. Set the new tty modes.
     */
    if (tcsetattr(0, TCSANOW, &newtty) < 0) {
        perror("tcsetattr: stdin");
        exit(1);
    }

    /*
     * 8. Now just sit in a loop reading from the keyboard and
     *    writing to the pseudo-tty, and reading from the 
     *    pseudo-tty and writing to the screen and the script file.
     */
    for (;;) {
        FD_ZERO(&readmask);
        FD_SET(master, &readmask);
        FD_SET(0, &readmask);
        nfd = master + 1;

        /*
         * 8a. Wait for something to read.
         */
        n = select(nfd, (int *) &readmask, (int *) 0, (int *) 0,
                   (struct timeval *) 0);

        if (n < 0) {
            perror("select");
            exit(1);
        }

        /*
         * 8b. The user typed something... read it and pass
         *      it on to the pseudo-tty.
         */
        if (FD_ISSET(0, &readmask)) {
            if ((n = read(0, buf, sizeof(buf))) < 0) {
                perror("read: stdin");
                exit(1);
            }

            /*
             * The user typed end-of-file; we're
             * done.
             */
            if (n == 0)
                finish(0);

            if (write(master, buf, n) != n) {
                perror("write: pty");
                exit(1);
            }
        }

        /*
         * 8c. There's output on the pseudo-tty... read it and
         *     pass it on to the screen and the script file.
         */
        if (FD_ISSET(master, &readmask)) {
            /*
             * The process died.
             */
            if ((n = read(master, buf, sizeof(buf))) <= 0)
                finish(0);

            fwrite(buf, sizeof(char), n, script);
            write(1, buf, n);
        }
    }
}

/*
 * ptyopen - start command on a pseudo-tty and return a file descriptor
 *       with which to speak to it.
 */
int
ptyopen(char *command, struct termios *ttymodes)
{
    char *p;
    pid_t pid;
    char *slavename;
    char *args[MAXARGS];
    int nargs, master, slave;

    /*
     * 9. Break the command into arguments.
     */
    nargs = 0;
    p = strtok(command, " \t\n");

    do {
        if (nargs == MAXARGS) {
            fprintf(stderr, "too many arguments.\n");
            return(-1);
        }
        
        args[nargs++] = p;
        p = strtok(NULL, " \t\n");
    } while (p != NULL);

    args[nargs] = NULL;

    /*
     * 10. Get a master pseudo-tty.
     */
    if ((master = open(mastername, O_RDWR)) < 0) {
        perror(mastername);
        return(-1);
    }

    /*
     * 11. Set the permissions on the slave.
     */
    if (grantpt(master) < 0) {
        perror("granpt");
        close(master);
        return(-1);
    }

    /*
     * 12. Unlock the slave.
     */
    if (unlockpt(master) < 0) {
        perror("unlockpt");
        close(master);
        return(-1);
    }

    /*
     * 13. Start a child process.
     */
    if ((pid = fork()) < 0) {
        perror("fork");
        close(master);
        return(-1);
    }

    /*
     * 14. The child process will open the slave, which will become
     *     its controlling terminal.
     */
    if (pid == 0) {
        /*
         * 14a. Get rid of our current controlling terminal.
         */
        setsid();

        /*
         * 14b. Get the name of the slave pseudo-tty.
         */
        if ((slavename = ptsname(master)) == NULL) {
            perror("ptsname");
            close(master);
            exit(1);
        }

        /*
         * 14c. Open the slave pseudo-tty.
         */
        if ((slave = open(slavename, O_RDWR)) < 0) {
            perror(slavename);
            close(master);
            exit(1);
        }

        /*
         * 14d. Push the hardware emulation module.
         */
        if (ioctl(slave, I_PUSH, "ptem") < 0) {
            perror("ioctl: ptem");
            close(master);
            close(slave);
            exit(1);
        }

        /*
         * 14e. Push the line discipline module.
         */
        if (ioctl(slave, I_PUSH, "ldterm") < 0) {
            perror("ioctl: ldterm");
            close(master);
            close(slave);
            exit(1);
        }

        /*
         * 14f. Copy the user's terminal modes to the slave
         *      pseudo-tty.
         */
        if (tcsetattr(slave, TCSANOW, ttymodes) < 0) {
            perror("tcsetattr: pty");
            close(master);
            close(slave);
            exit(1);
        }

        /*
         * 14g. Close the script file and the master; these
         *      are not needed in the slave.
         */
        fclose(script);
        close(master);

        /*
         * 14h. Set the slave to be our standard input, output,
         *      and error output.  Then get rid of the original
         *      file descriptor.
         */
        dup2(slave, 0);
        dup2(slave, 1);
        dup2(slave, 2);
        close(slave);

        /*
         * 14i. Execute the command.
         */
        execv(args[0], args);
        perror(args[0]);
        exit(1);
    }

    /*
     * 15. Return the file descriptor for communicating with
     *     the process to our caller.
     */
    return(master);
}
        
/*
 * finish - called when we're done.
 */
void
finish(int sig)
{
    time_t clock;

    /*
     * 16. Restore our original tty modes.
     */
    if (tcsetattr(0, TCSANOW, &origtty) < 0)
        perror("tcsetattr: stdin");

    /*
     * Print a finishing message.
     */
    time(&clock);
    fprintf(script, "\nScript finished at %s", ctime(&clock));
    printf("\nScript done, file is %s\n", filename);

    /*
     * 17. All done.
     */
    fclose(script);
    close(master);
    exit(0);
}
