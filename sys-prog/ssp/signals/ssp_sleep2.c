#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

static jmp_buf alrm_env;

static void sigalrm (int sig);

unsigned int ssp_sleep (unsigned int sec)
{
	if (sigset (SIGALRM, sigalrm) == SIG_ERR)
		return (sec);

	if (setjmp (alrm_env) == 0) {
		alarm (sec);
		pause ();
	}

	return (alarm (0));
}

static void sigalrm (int sig)
{
	longjmp (alrm_env, 1);
}
