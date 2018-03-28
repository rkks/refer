#include <stdio.h>
#include <signal.h>

#undef SIG_DFL
#undef SIG_IGN
#define SIG_DFL (int (*) ()) 0
#define SIG_IGN (int (*) ()) 1

static int ssig_action (int sig);

int main (void)
{
	int sig;

	ssignal (1, SIG_IGN);
	ssignal (2, ssig_action);
	ssignal (3, SIG_IGN);
	ssignal (4, ssig_action);
	ssignal (5, SIG_DFL);

	for (sig = 1; sig < 10; sig++)
		printf ("gsignal (%d) returns %d\n", sig, gsignal (sig));

	return (0);
}

static int ssig_action (int sig)
{
	printf ("Received software signal %d\n", sig);

	return (sig);
}
