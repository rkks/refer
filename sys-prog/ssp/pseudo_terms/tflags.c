#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "ssp.h"

int main (void)
{
	struct termios term;

	printf ("Getting terminal attributes\n");
	if (tcgetattr (STDIN_FILENO, &term) == -1)
		err_msg ("tcgetattr failed");

	printf ("Setting terminal attributes (TCSANOW)\n");
	if (tcsetattr (STDIN_FILENO, TCSANOW, &term) == -1)
		err_msg ("tcgetattr failed");

	printf ("Setting terminal attributes (TCSADRAIN)\n");
	if (tcsetattr (STDIN_FILENO, TCSADRAIN, &term) == -1)
		err_msg ("tcgetattr failed");

	printf ("Setting terminal attributes (TCSAFLUSH)\n");
	if (tcsetattr (STDIN_FILENO, TCSAFLUSH, &term) == -1)
		err_msg ("tcgetattr failed");

	printf ("Enabling XON/XOFF flow control\n");
	if (tcflow (STDIN_FILENO, TCOON) == -1)
		err_msg ("tcflow (TCOON) failed");

	printf ("Suspending output (^S)\n");
	if (tcflow (STDIN_FILENO, TCIOFF) == -1)
		err_msg ("tcflow (TCIOFF) failed");

	printf ("Resuming output (^Q)\n");
	if (tcflow (STDIN_FILENO, TCION) == -1)
		err_msg ("tcflow (TCION) failed");

	printf ("Disabling XON/XOFF flow control\n");
	if (tcflow (STDIN_FILENO, TCOOFF) == -1)
		err_msg ("tcflow (TCOOFF) failed");

	printf ("Reenabling XON/XOFF flow control\n");
	if (tcflow (STDIN_FILENO, TCOON) == -1)
		err_msg ("tcflow (TCOON) failed");

	printf ("Flushing read and write queues\n");
	if (tcflush (STDIN_FILENO, TCIOFLUSH) == -1)
		err_msg ("tcflush failed");

	return (0);
}
