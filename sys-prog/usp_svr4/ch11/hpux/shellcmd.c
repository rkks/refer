#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int     shellcmd(char *);
char    *strsignal(int);
void    prstat(int);

int
main(void)
{
    int status;
    char command[BUFSIZ];

    /*
     * Forever...
     */
    for (;;) {
        /*
         * Prompt for a command.
         */
        printf("Enter a command: ");

        /*
         * Read a command.  If NULL is returned, the
         * user typed CTRL-D, so exit.
         */
        if (fgets(command, sizeof(command), stdin) == NULL) {
            putchar('\n');
            exit(0);
        }

        /*
         * Strip off the trailing newline character
         * left by fgets.
         */
        command[strlen(command)-1] = '\0';

        /*
         * Execute the command and print the termination
         * status.
         */
        status = shellcmd(command);
        prstat(status);
        putchar('\n');
    }
}

/*
 * shellcmd - start a child process, and pass command to the shell.
 */
int
shellcmd(char *command)
{
    int status;
    pid_t p, pid;
    extern int errno;
    sigset_t mask, savemask;
    struct sigaction ignore, saveint, savequit;

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
         * Execute a shell with the command as argument.
         */
        execl("/bin/sh", "sh", "-c", command, 0);
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
     * Return the child process' termination status.
     */
    return(status);
}

/*
 * prstat - decode the termination status.
 */
void
prstat(int status)
{
    if (WIFEXITED(status)) {
        printf("Process terminated normally, exit status = %d.\n",
               WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
        printf("Process terminated abnormally, signal = %d (%s)",
               WTERMSIG(status), strsignal(WTERMSIG(status)));

        if (WCOREDUMP(status))
            printf(" -- core file generated.\n");
        else
            printf(".\n");
    }
    else if (WIFSTOPPED(status)) {
        printf("Process stopped, signal = %d (%s).\n",
               WSTOPSIG(status), strsignal(WSTOPSIG(status)));
    }
}

char *
strsignal(int sig)
{
    char *siglist[NSIG] = {
        "Unknown Signal",
        "Hangup",
        "Interrupt",
        "Quit",
        "Illegal Instruction",
        "Trace/Breakpoint Trap",
        "Abort",
        "Emulation Trap",
        "Arithmetic Exception",
        "Killed",
        "Bus Error",
        "Segmentation Fault",
        "Bad System Call",
        "Broken Pipe",
        "Alarm Clock",
        "Terminated",
        "User Signal 1",
        "User Signal 2",
        "Child Status Changed",
        "Power-Fail/Restart",
        "Virtual Timer Expired",
        "Profiling Timer Expired",
        "Pollable Event",
        "Window Size Change",
        "Stopped (signal)",
        "Stopped (user)",
        "Continued",
        "Stopped (tty input)",
        "Stopped (tty output)",
        "Urgent Socket Condition",
        "File Lock Lost"
    };

    return(sig < NSIG ? siglist[sig] : siglist[0]);
}
