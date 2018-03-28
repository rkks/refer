#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include "ssp.h"

#define DEF_USER "root"

int main (void)
{
	int fd [2];
	char *user;
	char buf [LINE_LEN];
	int i;
	char *msg [] = {
		"From: Solaris Systems Programming <nobody@localhost>\n",
		"",
		"Subject: Test email from pipemail\n\n",
		"Hi there,\n\n",
		"This email was sent from the pipemail\n",
		"program to show how to send data to an\n"
		"external program through a pipe.\n\n",
		"Bye.\n",
		NULL
	};

	if ((user = getenv ("MAIL_USER")) == NULL) {
		if ((user = cuserid (NULL)) == NULL)
			user = DEF_USER;
	}

	snprintf (buf, LINE_LEN, "To: %s\n", user);
	msg [1] = buf;

	if (pipe (fd) == -1)
		err_msg ("pipe failed");

	switch (fork ()) {
		case -1:
			err_msg ("fork failed");

		case 0:
			close (fd [1]);
			if (dup2 (fd [0], STDIN_FILENO) != STDIN_FILENO)
				err_msg ("dup to standard input failed");
			close (fd [0]);

			execl ("/usr/lib/sendmail", "sendmail", "-t", NULL);
			err_msg ("Can't exec sendmail");

		default:
			close (fd [0]);

			for (i = 0; msg [i] != NULL; i++) {
				if (writen (fd [1], msg [i], strlen (msg [i])) == -1)
					err_msg ("Can't write to sendmail");
			}

			close (fd [1]);
			wait (NULL);
			break;
	}

	return (0);
}
