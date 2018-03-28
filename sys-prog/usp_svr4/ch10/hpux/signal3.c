#include <signal.h>
#include <stdio.h>

void handler(int);
void psignal(int, char *);

int
main(void)
{
    printf("NOTE: HP-UX 10.x does not provide the psignal() function.\n");
    printf("      This example uses its own version of psignal().\n\n");

    /*
     * Send SIGUSR1 and SIGUSR2 to the handler function.
     */
    if (sigset(SIGUSR1, handler) == SIG_ERR) {
        fprintf(stderr, "cannot set handler for SIGUSR1\n");
        exit(1);
    }

    if (sigset(SIGUSR2, handler) == SIG_ERR) {
        fprintf(stderr, "cannot set handler for SIGUSR2\n");
        exit(1);
    }

    /*
     * Now wait for signals to arrive.
     */
    for (;;)
        pause();
}

/*
 * handler - handle a signal.
 */
void
handler(int sig)
{
    /*
     * Print out what we received.
     */
    psignal(sig, "Received signal");
}

void
psignal(int sig, char *str)
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

    fprintf(stderr, "%s %s\n", str, sig < NSIG ? siglist[sig] : siglist[0]);
}
