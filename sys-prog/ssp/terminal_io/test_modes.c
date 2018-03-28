#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <signal.h>
#include "ssp.h"

extern int tty_cbreak (int fd);
extern int tty_raw (int fd);
extern int tty_reset (int fd);
static void sig_handler (int sig);

int main (void)
{
	int i;
	char c;

	if (sigset (SIGINT, sig_handler) == SIG_ERR)
		err_msg ("sigset error");
	if (sigset (SIGQUIT, sig_handler) == SIG_ERR)
		err_msg ("sigset error");
	if (sigset (SIGTERM, sig_handler) == SIG_ERR)
		err_msg ("sigset error");

	if (tty_raw (STDIN_FILENO) == -1)
		err_msg ("tty_raw error");

	printf ("Enter raw mode characters, end with Backspace\n");
	while ((i = read (STDIN_FILENO, &c, 1)) == 1) {
		if ((c &= 255) == 0x08)
			break;
		printf ("%d\n", c);
	}

	if (tty_reset (STDIN_FILENO) == -1)
		err_msg ("tty_reset");
	if (i <= 0)
		err_msg ("read error");

	if (tty_cbreak (STDIN_FILENO) == -1)
		err_msg ("tty_cbreak error");

	printf ("\nEnter cbreak mode characters, end with INTR\n");
	while ((i = read (STDIN_FILENO, &c, 1)) == 1) {
		c &= 255;
		printf ("%d\n", c);
	}

	if (tty_reset (STDIN_FILENO) == -1)
		err_msg ("tty_reset");

	if (i <= 0)
		err_msg ("read error");

	return (0);
}

static void sig_handler (int sig)
{
	tty_reset (STDIN_FILENO);
	exit (0);
}
