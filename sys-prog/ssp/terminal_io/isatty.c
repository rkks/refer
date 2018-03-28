#include <stdio.h>
#include <termios.h>

static int ssp_isatty (int fd);

int main (void)
{
	int i;

	for (i = 0; i < 3; i++)
		printf ("fd %d: %s a tty\n", i, (ssp_isatty (i)) ? "is" : "is not");

	return (0);
}

static int ssp_isatty (int fd)
{
	struct termios term;

	return (tcgetattr (fd, &term) != -1);
}
