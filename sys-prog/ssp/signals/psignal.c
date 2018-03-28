#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <siginfo.h>
#include <sys/ucontext.h>
#include "ssp.h"

static void sigusr (int sig, siginfo_t *info, void *ucp);

int main (void)
{
	struct sigaction act;

	act.sa_sigaction = sigusr;
	sigemptyset (&act.sa_mask);
	act.sa_flags = SA_SIGINFO;

	if (sigaction (SIGUSR1, &act, NULL) == -1)
		err_msg ("sigaction (SIGUSR1) failed");
	if (sigaction (SIGUSR2, &act, NULL) == -1)
		err_msg ("sigaction (SIGUSR2) failed");

	for (;;)
		pause ();
}

static void sigusr (int sig, siginfo_t *info, void *ucp)
{
	if (sig == SIGUSR1)
		psignal (sig, "Received signal");
	else
		psiginfo (info, "Received signal");
}
