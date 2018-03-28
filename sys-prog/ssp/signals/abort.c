#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void abort (void)
{
	sigset_t set;
	struct sigaction action;

	sigaction (SIGABRT, NULL, &action);
	if ((action.sa_handler != SIG_DFL) && (action.sa_handler != SIG_IGN)) {
		sigprocmask (0, NULL, &set);
		if (sigismember (&set, SIGABRT)) {
			sigpending (&set);
			if (!sigismember (&set, SIGABRT))
				raise (SIGABRT);
			sigfillset (&set);
			sigdelset (&set, SIGABRT);
			sigprocmask (SIG_SETMASK, &set, NULL);
		}
		else
			raise (SIGABRT);
	}

	fflush (NULL);

	for (;;) {
		action.sa_handler = SIG_DFL;
		sigaction (SIGABRT, &action, NULL);

		sigfillset (&set);
		sigdelset (&set, SIGABRT);
		sigprocmask (SIG_SETMASK, &set, NULL);

		raise (SIGABRT);
	}
}
