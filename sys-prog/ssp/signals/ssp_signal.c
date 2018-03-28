#include <signal.h>
#include <errno.h>

void (*ssp_signal (int sig, void (*func) (int))) (int)
{
	struct sigaction new_action;
	struct sigaction old_action;

	new_action.sa_handler = func;
	new_action.sa_flags = SA_RESETHAND | SA_NODEFER;
	sigemptyset (&new_action.sa_mask);

	if (sig == SIGCHLD) {
		new_action.sa_flags |= SA_NOCLDSTOP;
		if (func == SIG_IGN)
			new_action.sa_flags |= SA_NOCLDWAIT;
	}

	if ((sig == SIGTSTP) || (sig == SIGTTIN) || (sig == SIGTTOU))
		new_action.sa_flags |= SA_RESTART;

	if (sigaction (sig, &new_action, &old_action) == -1)
		return (SIG_ERR);
	else
		return (old_action.sa_handler);
}
