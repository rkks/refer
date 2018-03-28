#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <ucontext.h>
#include <pwd.h>
#include "ssp.h"

static void sigterm (int sig, siginfo_t *info, void *uap);

int main (void)
{
	struct sigaction action;

	action.sa_sigaction = sigterm;
	action.sa_flags = SA_SIGINFO;
	sigemptyset (&action.sa_mask);

	if (sigaction (SIGTERM, &action, NULL) == -1)
		err_msg ("sigaction failed");

	printf ("Waiting...\n");
	pause ();

	return (0);
}

static void sigterm (int sig, siginfo_t *info, void *uap)
{
	struct passwd *pwent;

	if (info -> si_code != SI_NOINFO) {
		if ((pwent = getpwuid (info -> si_uid)) == NULL) {
			printf ("Ack!  Killed by user %ld, from process %ld\n",
				(long) info -> si_uid, (long) info -> si_pid);
		}
		else {
			printf ("Ack!  Killed by user %s, from process %ld\n",
				pwent -> pw_name, (long) info -> si_pid);
		}
	}
}
