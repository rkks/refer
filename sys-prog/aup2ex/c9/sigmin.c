/*
	Minimal defensive signal handling
	AUP2, Sec. 9.01.8

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
/*[clean_up]*/
static void clean_up(void)
{
	/*
		Clean-up code goes here --
		must be async-signal-safe.
	*/
}

static size_t strlen_safe(const char *s)
{
	size_t n = 0;
	while (*s++ != '\0')
		n++;
	return n;
}
/*[handler]*/
static void handler(int signum)
{
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

	clean_up();
	for (i = 0; sigmsg[i].signum > 0; i++)
		if (sigmsg[i].signum == signum) {
			(void)write(STDERR_FILENO, sigmsg[i].msg,
			  strlen_safe(sigmsg[i].msg));
			(void)write(STDERR_FILENO, "\n", 1);
			break;
		}
	_exit(EXIT_FAILURE);
}
/*[handle_signals]*/
static bool handle_signals(void)
{
	sigset_t set;
	struct sigaction act;

	ec_neg1( sigfillset(&set) )
	ec_neg1( sigprocmask(SIG_SETMASK, &set, NULL) )
	memset(&act, 0, sizeof(act));
	ec_neg1( sigfillset(&act.sa_mask) )
	act.sa_handler = SIG_IGN;
	ec_neg1( sigaction(SIGHUP, &act, NULL) )
	ec_neg1( sigaction(SIGINT, &act, NULL) )
	ec_neg1( sigaction(SIGQUIT, &act, NULL) )
	ec_neg1( sigaction(SIGPIPE, &act, NULL) )
	act.sa_handler = handler;
	ec_neg1( sigaction(SIGTERM, &act, NULL) )
	ec_neg1( sigaction(SIGBUS, &act, NULL) )
	ec_neg1( sigaction(SIGFPE, &act, NULL) )
	ec_neg1( sigaction(SIGILL, &act, NULL) )
	ec_neg1( sigaction(SIGSEGV, &act, NULL) )
	ec_neg1( sigaction(SIGSYS, &act, NULL) )
	ec_neg1( sigaction(SIGXCPU, &act, NULL) )
	ec_neg1( sigaction(SIGXFSZ, &act, NULL) )
	ec_neg1( sigemptyset(&set) )
	ec_neg1( sigprocmask(SIG_SETMASK, &set, NULL) )
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
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}

#define MAX_SIG 50

int main(void)
{
	sigset_t set;
	int x = 1, y;

	ec_neg1( sigfillset(&set) )
	ec_neg1( sigprocmask(SIG_SETMASK, &set, NULL) )
	y = x / 0;
#if 0
	int signum;
	pid_t pid[MAX_SIG + 1];
	int status;

	for (signum = 1; signum <= MAX_SIG; signum++) {
		ec_neg1( pid[signum] = fork() )
		if (pid[signum] == 0)
			process();
		sleep(1);
		printf("\nSending signal %d...\n", signum);
		if (kill(pid[signum], signum) == -1)
			printf("\tkill failed\n");
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
#endif
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
