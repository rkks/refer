#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "ssp.h"

static volatile sig_atomic_t sig;

static void sigusr (int sig_num);

int main (void)
{
	if (signal (SIGUSR1, sigusr) == SIG_ERR)
		err_msg ("Can't catch SIGUSR1");
	if (signal (SIGUSR2, sigusr) == SIG_ERR)
		err_msg ("Can't catch SIGUSR2");

	for (;;) {
		pause ();
		if (sig == SIGUSR1)
			printf ("Received SIGUSR1\n");
		else
			printf ("Received SIGUSR2\n");
	}
}

static void sigusr (int sig_num)
{
	if ((sig_num == SIGUSR1) || (sig_num == SIGUSR2))
		sig = sig_num;
	else
		err_dump ("Received signal %d", sig_num);
}
