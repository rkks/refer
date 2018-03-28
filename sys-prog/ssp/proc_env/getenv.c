#include <stdio.h>
#include <stdlib.h>
#include "ssp.h"

int main (void)
{
	char *s;
	char buf [LINE_LEN];

	if ((s = getenv ("HOME")) == NULL)
		err_quit ("HOME is not in the environment");
	printf ("Before putenv, HOME is %s\n", s);

	snprintf (buf, LINE_LEN, "HOME=where the heart is");
	if (putenv (buf) == -1)
		err_msg ("putenv failed");

	if ((s = getenv ("HOME")) == NULL)
		err_quit ("HOME is not in the environment");
	printf ("After putenv, HOME is %s\n", s);

	return (0);
}
