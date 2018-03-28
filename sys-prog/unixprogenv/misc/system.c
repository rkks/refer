/*
 * Safer version of system for interactive programs
 */
#include <signal.h>
#include <stdio.h>

system(s)	/* run command line s */
	char *s;
{
	int status, pid, w, tty;
	int (*istat)(), (*qstat)();
	extern char *progname;

	fflush(stdout);
	tty = open("/dev/tty", 2);
	if (tty == -1) {
		fprintf(stderr, "%s: can't open /dev/tty\n", progname);
		return -1;
	}
	if ((pid = fork()) == 0) {
		close(0); dup(tty);
		close(1); dup(tty);
		close(2); dup(tty);
		close(tty);
		execlp("sh", "sh", "-c", s, (char *) 0);
		exit(127);
	}
	close(tty);
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
