#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "ssp.h"

static char *print_csize (int size);

int main (void)
{
	struct termios term;
	int size;

	if (tcgetattr (STDIN_FILENO, &term) == -1)
		err_msg ("Can't get terminal attributes");
	size = term.c_cflag & CSIZE;
	printf ("Old size is %s\n", print_csize (size));

	term.c_cflag &= ~CSIZE;		/* Clear the size bits */
	term.c_cflag |= CS8;		/* Set 8 bits per character */

	if (tcsetattr (STDIN_FILENO, TCSANOW, &term) == -1)
		err_msg ("Can't set terminal attributes");

	if (tcgetattr (STDIN_FILENO, &term) == -1)
		err_msg ("Can't get terminal attributes");
	size = term.c_cflag & CSIZE;
	printf ("New size is %s\n", print_csize (size));

	return (0);
}

static char *print_csize (int size)
{
	switch (size) {
		case CS5:
			return ("5 bits/character");

		case CS6:
			return ("6 bits/character");

		case CS7:
			return ("7 bits/character");

		case CS8:
			return ("8 bits/character");

		default:
			return ("unknown");
	}
}
