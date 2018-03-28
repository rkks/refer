#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <syslog.h>
#include "ssp.h"

extern int daemon_proc;

void daemon_init (const char *prog_name, int inetd)
{
	pid_t pid;
	int fd;
	struct rlimit limits;

	if (!inetd) {
		if ((pid = fork ()) == -1)
			err_msg ("fork failed");
		else if (pid != 0)
			_exit (0);

		setsid ();

		if ((pid = fork ()) == -1)
			exit (1);
		else if (pid != 0)
			_exit (0);

		close (0);
		fd = open ("/dev/null", O_RDWR);
		dup2 (fd, 1);
		dup2 (fd, 2);
	}

	daemon_proc = 1;

	getrlimit (RLIMIT_NOFILE, &limits);
	if (limits.rlim_cur < limits.rlim_max)
		limits.rlim_cur = limits.rlim_max;
	setrlimit (RLIMIT_NOFILE, &limits);

	for (fd = 3; fd <= limits.rlim_cur; fd++)
		close (fd);

	chdir ("/");

	umask (0);

	openlog (prog_name, LOG_PID, LOG_DAEMON);
}
