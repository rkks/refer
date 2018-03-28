/*
	RTS siginfo_t example
	AUP2, Sec. 9.05.1

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

#ifdef FREEBSD
#define sival_int sigval_int
#define sival_ptr sigval_ptr
#endif

/*[handler]*/
static void handler(int signum, siginfo_t *info, void *context)
{
	printf("signal number: %d\n", info->si_signo);
	printf("sending process ID: %ld\n", (long)info->si_pid);
	printf("real user ID of sending process: %ld\n", (long)info->si_uid);
	switch (info->si_code) {
	case SI_USER:
		printf("Signal from user\n");
		break;
	case SI_QUEUE:
		printf("Signal from sigqueue; value = %d\n",
		  info->si_value.sival_int);
		break;
	case SI_TIMER:
		printf("Signal from timer expiration; value = %d\n",
		  info->si_value.sival_int);
		break;
	case SI_ASYNCIO:
		printf("Signal from asynchronous I/O completion; value = %d\n",
		  info->si_value.sival_int);
		break;
	case SI_MESGQ:
		printf("Signal from message arrival; value = %d\n",
		  info->si_value.sival_int);
		break;
	default:
		printf("Other signal\n");
	}
}
/*[main]*/
int main(void)
{
	struct sigaction act;
	union sigval val;

	memset(&act, 0, sizeof(act));
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = handler;
	ec_neg1( sigaction(SIGUSR1, &act, NULL) )
#if _POSIX_REALTIME_SIGNALS > 0
	ec_neg1( sigaction(SIGRTMIN, &act, NULL) )
#endif
	ec_neg1( kill(getpid(), SIGUSR1) )
	val.sival_int = 1234;
#if _POSIX_REALTIME_SIGNALS > 0
	ec_neg1( sigqueue(getpid(), SIGRTMIN, val) )
#endif
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
