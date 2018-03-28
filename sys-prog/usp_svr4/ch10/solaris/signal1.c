#include <signal.h>
#include <stdio.h>

void handler(int);

int
main(void)
{
    /*
     * Send SIGUSR1 and SIGUSR2 to the handler function.
     */
    if (signal(SIGUSR1, handler) == SIG_ERR) {
        fprintf(stderr, "cannot set handler for SIGUSR1\n");
        exit(1);
    }

    if (signal(SIGUSR2, handler) == SIG_ERR) {
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
