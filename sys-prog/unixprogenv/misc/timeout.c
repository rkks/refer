/* timeout:  set time limit on a process */
#include <stdio.h>
#include <signal.h>
int	pid;		/* child process id */
char *progname;

main(argc, argv)
	int argc;
	char *argv[];
{
	int sec = 10, status, onalarm();

	progname = argv[0];
	if (argc > 1 && argv[1][0] == '-') {
		sec = atoi(&argv[1][1]);
		argc--;
		argv++;
	}
	if (argc < 2)
		error("Usage: %s [-10] command", progname);
	if ((pid=fork()) == 0) {
		execvp(argv[1], &argv[1]);
		error("couldn't start %s", argv[1]);
	}
	signal(SIGALRM, onalarm);
	alarm(sec);
	if (wait(&status) == -1 || (status & 0177) != 0)
		error("%s killed", argv[1]);
	exit((status >> 8) & 0377);
}

onalarm()	/* kill child when alarm arrives */
{
	kill(pid, SIGKILL);
}

#include "error.c"
