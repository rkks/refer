#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stropts.h>
#include <termios.h>
#include <sys/types.h>
#include <poll.h>
#include "ssp.h"

extern int tty_raw (int fd);
extern void tty_atexit (void);
static void disable_echo (int fd);
static void loop (int master_fd, int ignore_eof);
static void run_driver (const char *driver);

int main (int argc, char **argv)
{
	int master_fd;
	int c;
	int ignore_eof;
	int interactive;
	int quiet;
	int err_flag;
	pid_t pid;
	char *driver;
	struct termios old_termios;
	struct winsize tty_winsize;

	ignore_eof = 0;
	interactive = isatty (STDIN_FILENO);
	quiet = 0;
	driver = NULL;
	err_flag = 0;
	opterr = 0;

	while ((c = getopt (argc, argv, "d:eiq")) != EOF) {
		switch (c) {
			case 'd':
				driver = optarg;
				break;

			case 'e':
				ignore_eof++;
				break;

			case 'i':
				interactive = 0;
				break;

			case 'q':
				quiet++;
				break;

			default:
				err_flag++;
				break;
		}
	}

	if ((err_flag) || (optind >= argc))
		err_quit ("Usage: pty [-d driver] [eiq] command [arg ...]");

	if (interactive) {
		if (tcgetattr (STDIN_FILENO, &old_termios) == -1)
			err_msg ("tcgetattr failed");

		if (ioctl (STDIN_FILENO, TIOCGWINSZ, &tty_winsize) == -1)
			err_msg ("TIOCGWINSZ failed");

		pid = pty_fork (&master_fd, &old_termios, &tty_winsize);
	}
	else
		pid = pty_fork (&master_fd, NULL, NULL);

	if (pid == -1)
		err_msg ("pty_fork failed");

	if (pid == 0) {
		if (quiet)
			disable_echo (STDIN_FILENO);

		if (execvp (argv [optind], &argv [optind]) == -1)
			err_msg ("pty: %s", argv [optind]);
	}

	if ((interactive) && (driver == NULL)) {
		if (tty_raw (STDIN_FILENO) == -1)
			err_msg ("tty_raw failed");
		if (atexit (tty_atexit) == -1)
			err_msg ("tty_atexit failed");
	}

	if (driver)
		run_driver (driver);

	loop (master_fd, ignore_eof);

	return (0);
}

static void disable_echo (int fd)
{
	struct termios term_attrs;

	if (tcgetattr (fd, &term_attrs) == -1)
		err_msg ("tcgetattr failed");

	term_attrs.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE | ECHONL);
	term_attrs.c_oflag &= ~(ONLCR);

	if (tcsetattr (fd, TCSANOW, &term_attrs) == -1)
		err_msg ("tcsetattr failed");
}

static void loop (int master_fd, int ignore_eof)
{
	ssize_t n;
	char buf [BUFSIZ];
	struct pollfd fds [2];

	fds [0].fd = STDIN_FILENO;
	fds [0].events = POLLIN;
	fds [0].revents = 0;
	fds [1].fd = master_fd;
	fds [1].events = POLLIN;
	fds [1].revents = 0;

	for (;;) {
		if (poll ((struct pollfd *) &fds, 2, INFTIM) == -1)
			err_msg ("poll failed");

		if (fds [0].revents & POLLIN) {
			if ((n = read (STDIN_FILENO, buf, BUFSIZ)) == -1)
				err_msg ("read from stdin failed");

			if (n == 0) {
				if (ignore_eof) {
					fds [0].events = 0;
					continue;
				}
				else
					break;
			}

			if (writen (master_fd, buf, n) == -1)
				err_msg ("writen to pty master failed");
		}

		if (fds [1].revents & POLLIN) {
			if ((n = read (master_fd, buf, BUFSIZ)) == -1)
				err_msg ("read from pty master failed");

			if (n == 0)
				break;

			if (writen (STDOUT_FILENO, buf, n) == -1)
				err_msg ("writen to stdout failed");
		}
	}
}

static void run_driver (const char *driver)
{
	int fd [2];

	if (pipe (fd) == -1)
		err_msg ("pipe failed");

	switch (fork ()) {
		case -1:
			err_msg ("fork failed");

		case 0:
			close (fd [1]);

			if (dup2 (fd [0], STDIN_FILENO) != STDIN_FILENO)
				err_msg ("dup2 failed for stdin");
			if (dup2 (fd [0], STDOUT_FILENO) != STDOUT_FILENO)
				err_msg ("dup2 failed for stdout");

			close (fd [0]);

			execlp (driver, driver, NULL);
			err_msg ("pty: %s", driver);

		default:
			close (fd [0]);

			if (dup2 (fd [1], STDIN_FILENO) != STDIN_FILENO)
				err_msg ("dup2 failed for stdin");
			if (dup2 (fd [1], STDOUT_FILENO) != STDOUT_FILENO)
				err_msg ("dup2 failed for stdout");

			close (fd [1]);
			break;
	}
}
