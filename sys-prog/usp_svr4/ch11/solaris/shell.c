#include <sys/types.h>
#include <sys/wait.h>
#include <libgen.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#define NARGS   64

int execute(char **, char *, char *);

int
main(void)
{
    char **cp;
    int n, status;
    char *args[NARGS];
    char command[BUFSIZ];
    char *infile, *outfile;

    /*
     * Set up bufsplit to parse the command line.
     */
    bufsplit(" \t\n", 0, NULL);

    /*
     * Forever...
     */
    for (;;) {
        /*
         * Prompt for a command.
         */
again:  printf("--> ");

        /*
         * Read a command.  If NULL is returned, the
         * user typed CTRL-D, so exit.
         */
        if (fgets(command, sizeof(command), stdin) == NULL) {
            putchar('\n');
            exit(0);
        }

        /*
         * Split the command into words.
         */
        n = bufsplit(command, NARGS, args);
        args[n] = NULL;

        /*
         * Ignore blank lines.
         */
        if (**args == '\0')
            continue;

        /*
         * Find any input and output redirections.
         */
        infile = NULL;
        outfile = NULL;

        for (cp = args; *cp != NULL; cp++) {
            if (strcmp(*cp, "<") == 0) {
                if (*(cp+1) == NULL) {
                    fprintf(stderr, "You must specify ");
                    fprintf(stderr, "an input file.\n");
                    goto again;
                }

                *cp++ = NULL;
                infile = *cp;
            }
            else if (strcmp(*cp, ">") == 0) {
                if (*(cp+1) == NULL) {
                    fprintf(stderr, "You must specify ");
                    fprintf(stderr, "an output file.\n");
                    goto again;
                }

                *cp++ = NULL;
                outfile = *cp;
            }
        }

        /*
         * Execute the command.
         */
        status = execute(args, infile, outfile);
    }
}

/*
 * execute - execute a command, possibly with input/output redirection
 */
int
execute(char **args, char *infile, char *outfile)
{
    int status;
    pid_t p, pid;
    int infd, outfd;
    extern int errno;
    sigset_t mask, savemask;
    struct sigaction ignore, saveint, savequit;

    infd = -1;
    outfd = -1;

    /*
     * If an input file was given, open it.
     */
    if (infile != NULL) {
        if ((infd = open(infile, O_RDONLY)) < 0) {
            perror(infile);
            return(-1);
        }
    }

    /*
     * If an output file was given, create it.
     */
    if (outfile != NULL) {
        if ((outfd = creat(outfile, 0666)) < 0) {
            perror(outfile);
            close(infd);
            return(-1);
        }
    }

    /*
     * Set up a sigaction structure to ignore signals.
     */
    sigemptyset(&ignore.sa_mask);
    ignore.sa_handler = SIG_IGN;
    ignore.sa_flags = 0;

    /*
     * Ignore keyboard signals; save old dispositions.
     */
    sigaction(SIGINT, &ignore, &saveint);
    sigaction(SIGQUIT, &ignore, &savequit);

    /*
     * Block SIGCHLD.
     */
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, &savemask);

    /*
     * Start a child process.
     */
    if ((pid = fork()) < 0)
        status = -1;

    /*
     * This code executes in the child process.
     */
    if (pid == 0) {
        /*
         * Restore signals to their original dispositions,
         * and restore the signal mask.
         */
        sigaction(SIGINT, &saveint, (struct sigaction *) 0);
        sigaction(SIGQUIT, &savequit, (struct sigaction *) 0);
        sigprocmask(SIG_SETMASK, &savemask, (sigset_t *) 0);

        /*
         * Perform output redirection.
         */
        if (infd > 0)
            dup2(infd, 0);

        if (outfd > 0)
            dup2(outfd, 1);

        /*
         * Execute the command.
         */
        execvp(*args, args);
        perror("exec");
        _exit(127);
    }

    /*
     * Wait for the child process to finish.
     */
    while (waitpid(pid, &status, 0) < 0) {
        /*
         * EINTR (interrupted system call) is okay; otherwise,
         * we got some error that we need to report back.
         */
        if (errno != EINTR) {
            status = -1;
            break;
        }
    }

    /*
     * Restore signals to their original dispositions,
     * and restore the signal mask.
     */
    sigaction(SIGINT, &saveint, (struct sigaction *) 0);
    sigaction(SIGQUIT, &savequit, (struct sigaction *) 0);
    sigprocmask(SIG_SETMASK, &savemask, (sigset_t *) 0);

    /*
     * Close file descriptors.
     */
    close(outfd);
    close(infd);

    /*
     * Return the child process' termination status.
     */
    return(status);
}

/*
 * The bufsplit() function on Solaris 2.4 is broken.  Remove the
 * "#ifdef notdef" and "#endif" lines to enable this version.
 */
#ifdef notdef
size_t
bufsplit(char *buf, size_t n, char **a)
{
    int i, nsplit;
    static char *splitch = "\t\n";

    if (buf != NULL && n == 0) {
        splitch = buf;
        return(1);
    }

    nsplit = 0;
    while (nsplit < n) {
        a[nsplit++] = buf;

        if ((buf = strpbrk(buf, splitch)) == NULL)
            break;

        *(buf++) = '\0';

        if (*buf == '\0')
            break;
    }

    buf = strrchr(a[nsplit-1], '\0');

    for (i=nsplit; i < n; i++)
        a[i] = buf;

    return(nsplit);
}
#endif
