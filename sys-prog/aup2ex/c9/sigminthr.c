/*
	Minimal defensive signal handling using sigwait (bad example)
	AUP2, Sec. 9.02.2

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "defs.h"
#include <pthread.h>

/*[clean_up]*/
static void clean_up(void)
{
	/*
		Clean-up code goes here --
		need not be async-signal-safe.
	*/
}
/*[]*/
// redition of earlier example, but not necessarily what you want in a
// multithreaded program
/*[sig_thread]*/
static void *sig_thread(void *arg)
{
	int signum;
	int i;
	struct {
		int signum;
		char *msg;
	} sigmsg[] = {
		{ SIGTERM, "Termination signal" },
		{ SIGBUS, "Access to undefined portion of a memory object" },
		{ SIGFPE, "Erroneous arithmetic operation" },
		{ SIGILL, "Illegal instruction" },
		{ SIGSEGV, "Invalid memory reference" },
		{ SIGSYS, "Bad system call" },
		{ SIGXCPU, "CPU-time limit exceeded" },
		{ SIGXFSZ, "File-size limit exceeded" },
		{ 0, NULL}
	};

	{
		int x = 1, y;

printf("about to divide by zero\n");
		y = x / 0;
	}
printf("still alive\n");
	while (true) {
		ec_rv( sigwait((sigset_t *)arg, &signum) )
		clean_up();
		for (i = 0; sigmsg[i].signum > 0; i++)
			if (sigmsg[i].signum == signum) {
				fprintf(stderr, "%s\n", sigmsg[i].msg);
				break;
			}
		_exit(EXIT_FAILURE);
	}
	return (void *)true; /* never get here */

EC_CLEANUP_BGN
	EC_FLUSH("sig_thread")
	return (void *)false;
EC_CLEANUP_END
}
/*[handler]*/
static void handler(int signum)
{
	printf("got %d in handler\n", signum);
}
/*[handle_signals]*/
static bool handle_signals(void)
{
	sigset_t *set;
	struct sigaction act;
	pthread_t tid;

	ec_null( set = malloc(sizeof(*set)) )
	ec_neg1( sigfillset(set) )
	ec_rv( pthread_sigmask(SIG_SETMASK, set, NULL) )
	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	ec_neg1( sigaction(SIGHUP, &act, NULL) )
	ec_neg1( sigaction(SIGINT, &act, NULL) )
	ec_neg1( sigaction(SIGQUIT, &act, NULL) )
	ec_neg1( sigaction(SIGPIPE, &act, NULL) )
	ec_neg1( sigemptyset(set) )
	ec_neg1( sigaddset(set, SIGTERM) )
	ec_neg1( sigaddset(set, SIGBUS) )
	ec_neg1( sigaddset(set, SIGFPE) )
	ec_neg1( sigaddset(set, SIGILL) )
	ec_neg1( sigaddset(set, SIGSEGV) )
	ec_neg1( sigaddset(set, SIGSYS) )
	ec_neg1( sigaddset(set, SIGXCPU) )
	ec_neg1( sigaddset(set, SIGXFSZ) )
	ec_rv( pthread_sigmask(SIG_SETMASK, set, NULL) )
	ec_rv( pthread_create(&tid, NULL, sig_thread, set) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/
void process(void)
{
	ec_false (handle_signals() )
	sleep(2);
	{
		int x = 1, y;

		//y = x / 0;
	}
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}

#define MAX_SIG 50

int main(void)
{
	int signum;
	pid_t pid[MAX_SIG + 1];
	int status;

	for (signum = 1; signum <= MAX_SIG; signum++) {
		ec_neg1( pid[signum] = fork() )
		if (pid[signum] == 0)
			process();
		sleep(1);
		printf("\nSending signal %d...\n", signum);
		//if (kill(pid[signum], signum) == -1)
		//	printf("\tkill failed\n");
		ec_neg1( waitpid(-1, &status, WUNTRACED) )
		if (WIFEXITED(status))
			printf("%ld exited normally\n", (long)pid[signum]);
		else if (WIFSIGNALED(status))
			printf("%ld exited with signal %d\n", (long)pid[signum],
			  WTERMSIG(status));
		else
			printf("wait returned with other status\n");
	}
	for (signum = 1; signum <= MAX_SIG; signum++)
		(void)kill(pid[signum], SIGKILL);
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
