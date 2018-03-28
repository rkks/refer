#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include "ssp.h"

static sigjmp_buf buf;
static volatile sig_atomic_t can_jump;

static void sigusr1 (int sig);
static void sigalrm (int sig);
extern int print_proc_mask (const char *msg);

int main (void)
{
	if (sigset (SIGUSR1, sigusr1) == SIG_ERR)
		err_msg ("sigset (SIGUSR1) failed");
	if (sigset (SIGALRM, sigalrm) == SIG_ERR)
		err_msg ("sigset (SIGALRM) failed");

	print_proc_mask ("Starting main");

	if (sigsetjmp (buf, 1) != 0) {
		print_proc_mask ("Exiting main");
		exit (0);
	}

	can_jump = 1;

	for (;;)
		pause ();
}

static void sigusr1 (int sig)
{
	time_t now;

	if (!can_jump)
		return;

	print_proc_mask ("Starting sigusr1");

	alarm (2);

	now = time (NULL);
	for (;;)
		if (time (NULL) > now + 5)
			break;

	print_proc_mask ("Exiting sigusr1");

	can_jump = 0;
	siglongjmp (buf, 1);
}

static void sigalrm (int sig)
{
	print_proc_mask ("In sigalrm");
}
