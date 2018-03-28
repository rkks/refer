#include <unistd.h>
#include <termios.h>
#include "ssp.h"

int main (void)
{
	struct termios term;

	if (tcgetattr (STDIN_FILENO, &term) == -1)
		err_msg ("Can't get terminal attributes");

	term.c_cc [VINTR] = CTRL('l');			/* INTR is now Control-L */
	term.c_cc [VKILL] = _POSIX_VDISABLE;	/* Disable KILL */

	if (tcsetattr (STDIN_FILENO, TCSANOW, &term) == -1)
		err_msg ("Can't set terminal attributes");

	return (0);
}
