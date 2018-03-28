#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stropts.h>
#include <termios.h>
#include "ssp.h"

static int ptm_open (void)
{
	int fd;

	if ((fd = open ("/dev/ptmx", O_RDWR)) == -1)
		return (-1);

	if (grantpt (fd) == -1) {
		close (fd);
		return (-1);
	}

	if (unlockpt (fd) == -1) {
		close (fd);
		return (-1);
	}

	return (fd);
}

static int pts_open (int master_fd)
{
	char *name;
	int slave_fd;

	if ((name = ptsname (master_fd)) == NULL) {
		close (master_fd);
		return (-1);
	}

	if ((slave_fd = open (name, O_RDWR)) == -1) {
		close (master_fd);
		return (-1);
	}

	if (ioctl (slave_fd, I_PUSH, "ptem") == -1) {
		close (master_fd);
		close (slave_fd);
		return (-1);
	}

	if (ioctl (slave_fd, I_PUSH, "ldterm") == -1) {
		close (master_fd);
		close (slave_fd);
		return (-1);
	}

	if (ioctl (slave_fd, I_PUSH, "ttcompat") == -1) {
		close (master_fd);
		close (slave_fd);
		return (-1);
	}

	return (slave_fd);
}

pid_t pty_fork (int *master_fdp, const struct termios *slave_termios,
	const struct winsize *slave_winsize)
{
	int master_fd;
	int slave_fd;
	pid_t pid;

	if ((master_fd = ptm_open ()) == -1)
		return (-1);

	switch (pid = fork ()) {
		case -1:
			return (-1);

		case 0:
			if (setsid () == -1)
				err_msg ("setsid failed");

			if ((slave_fd = pts_open (master_fd)) == -1)
				err_msg ("Can't open pty slave");
			close (master_fd);

			if (slave_termios != NULL) {
				if (tcsetattr (slave_fd, TCSANOW, slave_termios) == -1)
					err_msg ("Can't set pty slave's attributes");
			}

			if (slave_winsize != NULL) {
				if (ioctl (slave_fd, TIOCSWINSZ, slave_winsize) == -1)
					err_msg ("Can't set pty slave's window size");
			}

			if (dup2 (slave_fd, STDIN_FILENO) != STDIN_FILENO)
				err_msg ("Can't dup2 stdin");
			if (dup2 (slave_fd, STDOUT_FILENO) != STDOUT_FILENO)
				err_msg ("Can't dup2 stdout");
			if (dup2 (slave_fd, STDERR_FILENO) != STDERR_FILENO)
				err_msg ("Can't dup2 stderr");
			if (slave_fd > STDERR_FILENO)
				close (slave_fd);

			_exit (0);

		default:
			*master_fdp = master_fd;
			return (pid);
	}
}
