#include	<sys/types.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<signal.h>
#include	"ourhdr.h"

static void	sig_hup(int);
static void	pr_ids(char *);

int
main(void)
{
	char	c;
	pid_t	pid;

	pr_ids("parent");
	if ( (pid = fork()) < 0)
		err_sys("fork error");

	else if (pid > 0) {	/* parent */
		sleep(5);		/* sleep to let child stop itself */
		exit(0);		/* then parent exits */

	} else {			/* child */
		pr_ids("child");
		signal(SIGHUP, sig_hup);	/* establish signal handler */
		kill(getpid(), SIGTSTP);	/* stop ourself */
		pr_ids("child");	/* this prints only if we're continued */
		if (read(0, &c, 1) != 1)
			printf("read error from control terminal, errno = %d\n", errno);
		exit(0);
	}
}

static void
sig_hup(int signo)
{
	printf("SIGHUP received, pid = %d\n", getpid());
	return;
}

static void
pr_ids(char *name)
{
	printf("%s: pid = %d, ppid = %d, pgrp = %d\n",
			name, getpid(), getppid(), getpgrp());
	fflush(stdout);
}
