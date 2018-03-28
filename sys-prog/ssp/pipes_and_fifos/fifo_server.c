#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/param.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include "ssp.h"

#define WELL_KNOWN_FIFO "/tmp/ssp_fifo"

static void rm_fifo (void);

int main (void)
{
	int n;
	int fd;
	int *fd_pid;
	char buf [LINE_LEN];
	char cmd [LINE_LEN];
	char path [MAXPATHLEN];
	pid_t pid;
	double num;
	double res;

	if (unlink (WELL_KNOWN_FIFO) == -1) {
		if (errno != ENOENT)
			err_msg ("unlink failed");
	}

	if (mkfifo (WELL_KNOWN_FIFO, FIFO_PERMS) == -1)
		err_msg ("mkfifo failed");

	if (atexit (rm_fifo) == -1)
		err_msg ("atexit failed");

	if ((fd = open (WELL_KNOWN_FIFO, O_RDONLY)) == -1)
		err_msg ("open failed");

	if ((fd_pid = calloc (MAXPID, sizeof (int))) == NULL)
		err_msg ("calloc failed");

	if (sigset (SIGPIPE, SIG_IGN) == SIG_ERR)
		err_msg ("sigset failed");

	while ((n = read (fd, buf, LINE_LEN)) > 0) {
		buf [n] = '\0';
		sscanf (buf, "%ld %s %lf", (long *) &pid, cmd, &num);

		if (strcmp (cmd, "open") == 0) {
			if (fd_pid [pid] == 0) {
				snprintf (path, MAXPATHLEN, "%s.%ld", WELL_KNOWN_FIFO,
					(long) pid);
				if (unlink (path) == -1) {
					if (errno != ENOENT) {
						err_ret ("unlink failed");
						continue;
					}
				}

				if (mkfifo (path, FIFO_PERMS) == -1) {
					err_ret ("mkfifo failed");
					continue;
				}

				if ((fd_pid [pid] = open (path, O_WRONLY)) == -1) {
					err_ret ("open failed");
					fd_pid [pid] = 0;
					continue;
				}
			}
		}

		if (strcmp (cmd, "sqrt") == 0) {
			res = sqrt (num);
			snprintf (buf, LINE_LEN, "%lf\n", res);
			n = strlen (buf);
			if (writen (fd_pid [pid], buf, n) == -1) {
				err_ret ("writen failed");
				close (fd_pid [pid]);
				fd_pid [pid] = 0;
			}
		}

		if (strcmp (cmd, "quit") == 0) {
			close (fd_pid [pid]);
			fd_pid [pid] = 0;
		}
	}

	return (0);
}

static void rm_fifo (void)
{
	unlink (WELL_KNOWN_FIFO);
}
