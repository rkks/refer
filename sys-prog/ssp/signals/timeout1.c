#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "ssp.h"

static volatile sig_atomic_t flag = 0;

static void sigalrm (int sig);

int main (void)
{
	char buf [BUFSIZ];
	char *def_string = "Hello, World!";

	if (sigset (SIGALRM, sigalrm) == SIG_ERR)
		err_msg ("sigset failed");

	alarm (10);

	printf ("Enter a greeting: ");
	if (fgets (buf, sizeof (buf), stdin) != NULL)
		buf [strlen (buf) - 1] = '\0';

	alarm (0);

	if (flag == 1) {
		strcpy (buf, def_string);
		printf ("\n");
	}

	printf ("Using greeting: \"%s\"\n", buf);

	return (0);
}

static void sigalrm (int sig)
{
	flag = 1;
}
