#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t flag;

static void sigtstp (int sig);

int main (void)
{
	if (sigset (SIGTSTP, SIG_IGN) == SIG_DFL)
		sigset (SIGTSTP, sigtstp);

	while (flag == 0)
		pause ();

	printf ("Program continued - we're done.\n");

	return (0);
}

static void sigtstp (int sig)
{
	sigset_t mask;

	printf ("\n");

	sigemptyset (&mask);
	sigaddset (&mask, SIGTSTP);
	sigprocmask (SIG_UNBLOCK, &mask, NULL);

	sigset (SIGTSTP, SIG_DFL);
	raise (SIGTSTP);
	sigset (SIGTSTP, sigtstp);

	flag++;
}
