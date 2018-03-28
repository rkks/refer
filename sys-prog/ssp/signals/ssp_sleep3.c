#include <unistd.h>
#include <signal.h>

static void sigalrm (int sig)
{
	/* Do nothing */
}

unsigned int ssp_sleep (unsigned int sleep_time)
{
	int alarm_flag;
	unsigned int unslept;
	unsigned int alarm_time;
	unsigned int remainder;
	struct sigaction act;
	struct sigaction saved_act;
	sigset_t alarm_mask;
	sigset_t set;
	sigset_t saved_set;

	if (sleep_time == 0)
		return (0);

	alarm_time = alarm (0);
	act.sa_handler = sigalrm;
	act.sa_flags = 0;
	sigemptyset (&act.sa_mask);
	sigaction (SIGALRM, &act, &saved_act);

	alarm_flag = 0;
	remainder = 0;

	if (alarm_time != 0) {
		if (alarm_time > sleep_time) {
			alarm_time -= sleep_time;
			alarm_flag = 1;
		}
		else {
			remainder = sleep_time - alarm_time;
			sleep_time = alarm_time;
			alarm_time = 0;
			alarm_flag = -1;
			sigaction (SIGALRM, &saved_act, NULL);
		}
	}

	sigemptyset (&alarm_mask);
	sigaddset (&alarm_mask, SIGALRM);
	sigprocmask (SIG_BLOCK, &alarm_mask, &saved_set);

	set = saved_set;
	sigdelset (&set, SIGALRM);
	alarm (sleep_time);
	sigsuspend (&set);
	unslept = alarm (0);

	sigprocmask (SIG_SETMASK, &saved_set, NULL);

	if (alarm_flag >= 0)
		sigaction (SIGALRM, &saved_act, NULL);

	if ((alarm_flag > 0) || ((alarm_flag < 0) && (unslept != 0)))
		alarm (alarm_time + unslept);

	return (remainder + unslept);
}
