#include <unistd.h>
#include <signal.h>

static void sigalrm (int sig);

unsigned int ssp_sleep (unsigned int sec)
{
	if (sigset (SIGALRM, sigalrm) == SIG_ERR)
		return (sec);

	alarm (sec);
	pause ();
	return (alarm (0));
}

static void sigalrm (int sig)
{
	/* Do nothing */
}
