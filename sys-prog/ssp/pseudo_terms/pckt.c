#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stropts.h>
#include <termios.h>
#include <sys/types.h>
#include <poll.h>
#include <sys/stream.h>
#include "ssp.h"

extern int tty_raw (int fd);
extern void tty_atexit (void);
static void disable_echo (int fd);
static void loop (int master_fd, int ignore_eof);

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
		err_quit ("Usage: pckt [-d driver] [eiq] command [arg ...]");

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
			err_msg ("pckt: %s", argv [optind]);
	}

	if ((interactive) && (driver == NULL)) {
		if (tty_raw (STDIN_FILENO) == -1)
			err_msg ("tty_raw failed");
		if (atexit (tty_atexit) == -1)
			err_msg ("tty_atexit failed");
	}

	if (ioctl (master_fd, I_PUSH, "pckt") == -1)
		err_msg ("Can't push pckt module");

	loop (master_fd, ignore_eof);

	return (0);
}

static void disable_echo (int fd)
{
	struct termios term_attrs;

	if (tcgetattr (fd, &term_attrs) == -1)
		err_msg ("tcgetattr failed");

	term_attrs.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHOCTL | ECHOKE | ECHONL);
	term_attrs.c_oflag &= ~(ONLCR | OCRNL);

	if (tcsetattr (fd, TCSANOW, &term_attrs) == -1)
		err_msg ("tcsetattr failed");
}

static void loop (int master_fd, int ignore_eof)
{
	ssize_t n;
	char buf [BUFSIZ];
	struct pollfd fds [2];
	char control_buf [BUFSIZ];
	char data_buf [BUFSIZ];
	int flags;
	struct strbuf control;
	struct strbuf data;
	struct iocblk *ioc;
	struct termios *term;
	unsigned char msg_type;
	int i;

	fds [0].fd = STDIN_FILENO;
	fds [0].events = POLLIN;
	fds [0].revents = 0;
	fds [1].fd = master_fd;
	fds [1].events = POLLIN;
	fds [1].revents = 0;

	control.buf = control_buf;
	control.maxlen = BUFSIZ;
	data.buf = data_buf;
	data.maxlen = BUFSIZ;

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
			flags = 0;
			if ((n = getmsg (master_fd, &control, &data, &flags)) == -1)
				err_msg ("getmsg from pty master failed");

			msg_type = control.buf [0];

			switch (msg_type) {
				case M_DATA:
					if (writen (STDOUT_FILENO, data.buf, data.len) == -1)
						err_msg ("writen to stdout failed");
					break;

				case M_FLUSH:
					fprintf (stderr, "pckt: pty slave flushed its queues\n");
					break;

				case M_STOPI:
					fprintf (stderr, "pckt: pty slave suspended output\n");
					break;

				case M_STARTI:
					fprintf (stderr, "pckt: pty slave resumed output\n");
					break;

				case M_STOP:
					fprintf (stderr, "pckt: pty slave disabled XON/XOFF "
						"flow control\n");
					break;

				case M_START:
					fprintf (stderr, "pckt: pty slave enabled XON/XOFF "
						"flow control\n");
					break;

				case M_IOCTL:
					ioc = (struct iocblk *) &data.buf [0];
					switch (ioc -> ioc_cmd) {
						case TCSBRK:
							fprintf (stderr, "pckt: pty slave sent BREAK\n");
							goto out;

						case TCSETS:
						case TCSETSW:
						case TCSETSF:
							fprintf (stderr, "pckt: pty slave changed "
								"terminal attributes\n");
							term = (struct termios *)
								&data.buf [sizeof (struct iocblk)];
							fprintf (stderr, "  term.c_iflag = %04x\n",
								term -> c_iflag);
							fprintf (stderr, "  term.c_oflag = %04x\n",
								term -> c_oflag);
							fprintf (stderr, "  term.c_cflag = %04x\n",
								term -> c_cflag);
							fprintf (stderr, "  term.c_lflag = %04x\n",
								term -> c_lflag);
							fprintf (stderr, "  term.c_cc = ");
							for (i = 0; i < NCCS; i++)
								fprintf (stderr, "%02x ", term -> c_cc [i]);
							fprintf (stderr, "\n");
							break;

						default:
							fprintf (stderr, "pckt: Unrecognised ioc_cmd: "
								"%04x\n", ioc -> ioc_cmd);
							fprintf (stderr, "  ioc -> ioc_cmd = %04x\n",
								ioc -> ioc_cmd);
							fprintf (stderr, "  ioc -> ioc_id = %04x\n",
								ioc -> ioc_id);
							fprintf (stderr, "  ioc -> ioc_flag = %04x\n",
								ioc -> ioc_flag);
							fprintf (stderr, "  ioc -> ioc_count = %04x\n",
								ioc -> ioc_count);
							fprintf (stderr, "  ioc -> ioc_rval = %04x\n",
								ioc -> ioc_rval);
							fprintf (stderr, "  ioc -> ioc_error = %04x\n",
								ioc -> ioc_error);
							break;
					}
					break;

				default:
					fprintf (stderr, "pckt: Unrecognised message type: "
						"%02x\n", msg_type);
					break;
			}
		}
	}

out:
	;
}
