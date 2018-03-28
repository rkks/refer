#include <signal.h>

system(s)	/* run command line s */
	char *s;
{
	int status, pid, w, tty;
	int (*istat)(), (*qstat)();

	...
	if ((pid = fork()) == 0) {
		...
		execlp("sh", "sh", "-c", s, (char *) 0);
		exit(127);
	}
	...
	istat = signal(SIGINT, SIG_IGN);
	qstat = signal(SIGQUIT, SIG_IGN);
	while ((w = wait(&status)) != pid && w != -1)
		;
	if (w == -1)
		status = -1;
	signal(SIGINT, istat);
	signal(SIGQUIT, qstat);
	return status;
}
