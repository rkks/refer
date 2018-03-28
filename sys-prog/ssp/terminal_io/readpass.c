#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ssp.h"

static char *readpass (const char *prompt);

int main (void)
{
	char *pass;

	if ((pass = readpass ("Enter password: ")) == NULL)
		err_msg ("readpass error");
	printf ("Password = %s\n", pass);

	while (*pass)
		*pass++ = '\0';

	return (0);
}

static char *readpass (const char *prompt)
{
	struct termios old_term;
	struct termios new_term;
	int fd;
	static char buf [PASS_MAX + 1];
	int n;

	if ((fd = open (ctermid (NULL), O_RDWR)) == -1)
		return (NULL);

	tcgetattr (fd, &old_term);
	new_term = old_term;
	new_term.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
	tcsetattr (fd, TCSAFLUSH, &new_term);

	writen (fd, prompt, strlen (prompt));

	memset (buf, 0, PASS_MAX + 1);
	read (fd, buf, PASS_MAX);
	writen (fd, "\n", 1);
	n = strlen (buf) - 1;
	if (buf [n] == '\n')
		buf [n] = '\0';

	tcsetattr (fd, TCSAFLUSH, &old_term);
	close (fd);

	return (buf);
}
