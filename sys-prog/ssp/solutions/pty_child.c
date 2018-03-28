#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include "ssp.h"

static volatile sig_atomic_t got_sigterm;
static volatile sig_atomic_t got_sigwinch;

static void print_winsize (int fd);
static void sigterm (int sig);
static void sigwinch (int sig);

int main (void)
{
	if (sigset (SIGTERM, sigterm) == SIG_ERR)
		err_msg ("sigset failed");
	if (sigset (SIGWINCH, sigwinch) == SIG_ERR)
		err_msg ("sigset failed");

	got_sigterm = 0;
	got_sigwinch = 0;

	for (;;) {
		pause ();
		if (got_sigterm) {
			printf ("SIGTERM received\n");
			got_sigterm = 0;
		}

		if (got_sigwinch) {
			printf ("SIGWINCH received\n");
			print_winsize (STDIN_FILENO);
			got_sigwinch = 0;
		}
	}
}

static void print_winsize (int fd)
{
	struct winsize size;

	if (ioctl (fd, TIOCGWINSZ, &size) == -1)
		err_msg ("ioctl failed");

	printf ("%d columns by %d rows (%d pixels by %d pixels)\n",
		size.ws_col, size.ws_row, size.ws_xpixel, size.ws_ypixel);
}

static void sigterm (int sig)
{
	got_sigterm = 1;
}

static void sigwinch (int sig)
{
	got_sigwinch = 1;
}
