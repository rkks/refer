#include <unistd.h>
#include <termios.h>
#include "ssp.h"

int main (void)
{
	struct termios term;

	if (tcgetattr (STDIN_FILENO, &term) == -1)
		err_msg ("Can't get terminal attributes");

	if (cfsetispeed (&term, B115200) == -1)
		err_msg ("Can't set output baud rate");

	if (tcsetattr (STDIN_FILENO, TCSANOW, &term) == -1)
		err_msg ("Can't set terminal attributes");

	if (tcgetattr (STDIN_FILENO, &term) == -1)
		err_msg ("Can't get terminal attributes");

	if (cfgetispeed (&term) != B115200)
		err_msg ("115200 baud not supported");

	return (0);
}
