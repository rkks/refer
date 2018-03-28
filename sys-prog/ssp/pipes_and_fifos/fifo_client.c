#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/param.h>
#include <errno.h>
#include <string.h>
#include "ssp.h"

#define WELL_KNOWN_FIFO "/tmp/ssp_fifo"

int main (void)
{
	pid_t pid;
	int wk_fd;
	int my_fd;
	int n;
	char buf [LINE_LEN];
	char path [MAXPATHLEN];

	if ((wk_fd = open (WELL_KNOWN_FIFO, O_WRONLY)) == -1)
		err_msg ("Can't open well known FIFO");

	pid = getpid ();

	snprintf (buf, LINE_LEN, "%d open\n", pid);
	n = strlen (buf);

	if (writen (wk_fd, buf, n) == -1)
		err_msg ("writen failed");

	snprintf (path, MAXPATHLEN, "%s.%d", WELL_KNOWN_FIFO, pid);

	while ((my_fd = open (path, O_RDONLY)) == -1) {
		if (errno != ENOENT)
			err_msg ("Can't open my FIFO");
	}

	if (unlink (path) == -1)
		err_msg ("unlink failed");

	printf ("Enter a number: ");
	fflush (stdout);

	while (fgets (path, LINE_LEN, stdin) != NULL) {
		snprintf (buf, LINE_LEN, "%d sqrt %s", pid, path);
		n = strlen (buf);
		if (writen (wk_fd, buf, n) == -1)
			err_msg ("writen failed");

		if ((n = read (my_fd, buf, LINE_LEN)) == -1)
			err_msg ("Can't read my FIFO");

		if (n == 0)
			err_msg ("Server closed my pipe");

		buf [n] = '\0';
		if (fputs (buf, stdout) == EOF)
			err_msg ("fputs failed");

		printf ("Enter a number: ");
		fflush (stdout);
	}

	if (ferror (stdin))
		err_msg ("fgets from standard input failed");

	snprintf (buf, LINE_LEN, "%d close\n", pid);
	n = strlen (buf);

	if (writen (wk_fd, buf, n) == -1)
		err_msg ("writen failed");

	printf ("\n");
	close (wk_fd);
	close (my_fd);

	return (0);
}
