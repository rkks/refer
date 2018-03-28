#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "ssp.h"

extern unsigned int ssp_sleep (unsigned int sec);
static void sigint (int sig);

int main (void)
{
	unsigned int unslept;

	if (sigset (SIGINT, sigint) == SIG_ERR)
		err_msg ("sigset failed");

	unslept = ssp_sleep (2);
	printf ("ssp_sleep returned %u\n", unslept);

	return (0);
}

static void sigint (int sig)
{
	int i;
	volatile int j;

	printf ("\nStarting SIGINT signal handler\n");
	for (i = 0; i < 10000000; i++)
		j += i * i;
	printf ("Returning from SIGINT signal handler\n");
}
