#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include "ssp.h"

int print_proc_mask (const char *msg)
{
	sigset_t set;
	int old_errno;
	int i;
	char buf [SIG2STR_MAX];

	old_errno = errno;
	if (sigprocmask (0, NULL, &set) == -1)
		return (-1);

	if (msg)
		printf ("%s: ", msg);

	snprintf (buf, SIG2STR_MAX, "SIG");
	for (i = 1; i < MAXSIG; i++) {
		if (sigismember (&set, i)) {
			sig2str (i, &buf [3]);
			printf ("%s ", buf);
		}
	}
	printf ("\n");

	errno = old_errno;

	return (0);
}
