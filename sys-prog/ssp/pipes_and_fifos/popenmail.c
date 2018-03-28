#include <stdio.h>
#include <stdlib.h>
#include "ssp.h"

#define DEF_USER "root"

int main (void)
{
	FILE *fp;
	char *user;
	char buf [LINE_LEN];
	int i;
	char *msg [] = {
		"From: Solaris Systems Programming <nobody@localhost>\n",
		"",
		"Subject: Test email from popenmail\n\n",
		"Hi there,\n\n",
		"This email was sent from the popenmail\n",
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

	if ((fp = popen ("/usr/lib/sendmail -t", "w")) == NULL)
		err_msg ("popen failed");

	for (i = 0; msg [i] != NULL; i++) {
		if (fputs (msg [i], fp) == EOF)
			err_msg ("Can't fputs to sendmail");
	}

	pclose (fp);

	return (0);
}
