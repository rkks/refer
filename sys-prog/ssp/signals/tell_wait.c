#include <sys/types.h>
#include <signal.h>

static sigset_t new_mask;
static sigset_t old_mask;

int ssp_tell_wait_init (void)
{
	sigemptyset (&new_mask);
	sigaddset (&new_mask, SIGUSR1);
	sigaddset (&new_mask, SIGUSR2);
	return (sigprocmask (SIG_BLOCK, &new_mask, &old_mask));
}

int ssp_tell_child (pid_t pid)
{
	return (kill (pid, SIGUSR1));
}

int ssp_tell_parent (pid_t pid)
{
	return (kill (pid, SIGUSR2));
}

int ssp_wait_child (void)
{
	int sig;

	return (sigwait (&new_mask, &sig));
}

int ssp_wait_parent (void)
{
	int sig;

	return (sigwait (&new_mask, &sig));
}

int ssp_tell_wait_fin (void)
{
	return (sigprocmask (SIG_SETMASK, &old_mask, NULL));
}
