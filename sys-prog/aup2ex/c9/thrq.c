/*
	Queued-signals example using threads
	AUP2, Sec. 9.05.5

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
/*
	Won't work on some imperfect implementations of POSIX threads, such as
	some versions of Linux threads, because signals ququed for a process
	are not	properly distributed to a thread.
*/
#include "defs.h"

#if _POSIX_REALTIME_SIGNALS > 0

#include <pthread.h>

/*[top]*/
#define MYSIG_COUNT SIGRTMIN
#define MYSIG_STOP SIGRTMIN + 1
/*[sig_thread]*/
static void *sig_thread(void *arg)
{
	int signum;
	siginfo_t info;

	do {
		signum = sigwaitinfo((sigset_t *)arg, &info);
		if (signum == MYSIG_COUNT)
			printf("Got MYSIG_COUNT; value: %s\n",
			  (char *)info.si_value.sival_ptr);
		else if (signum == MYSIG_STOP) {
			printf("Got MYSIG_STOP; terminating thread\n");
			return (void *)true;
		}
		else
			printf("Got %d\n", signum);
	} while (signum != -1 || errno == EINTR);
	EC_FAIL

EC_CLEANUP_BGN
	EC_FLUSH("sig_thread")
	return (void *)false;
EC_CLEANUP_END
}
/*[handler]*/
static void dummy_handler(int signum, siginfo_t *info, void *context)
{
}
/*[main]*/
int main(void)
{
	sigset_t set;
	struct sigaction act;
	union sigval value;
	pthread_t tid;

#ifdef LINUX
	printf("Warning: May hang -- see comment at the top of c9/thrq.c\n");
#endif
	ec_neg1( sigemptyset(&set) )
	ec_neg1( sigaddset(&set, MYSIG_COUNT) )
	ec_neg1( sigaddset(&set, MYSIG_STOP) )
	ec_rv( pthread_sigmask(SIG_SETMASK, &set, NULL) )
	memset(&act, 0, sizeof(act));
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = dummy_handler;
	ec_neg1( sigaction(MYSIG_COUNT, &act, NULL) )
	ec_neg1( sigaction(MYSIG_STOP, &act, NULL) )
	value.sival_ptr = "One";
	ec_neg1( sigqueue(getpid(), MYSIG_COUNT, value) )
	value.sival_ptr = "Two";
	ec_neg1( sigqueue(getpid(), MYSIG_COUNT, value) )
	value.sival_ptr = "Three";
	ec_neg1( sigqueue(getpid(), MYSIG_COUNT, value) )
	value.sival_ptr = NULL;
	ec_neg1( sigqueue(getpid(), MYSIG_STOP, value) )
	ec_rv( pthread_create(&tid, NULL, sig_thread, &set) )
	ec_rv( pthread_join(tid, NULL) )
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/

#else /* _POSIX_REALTIME_SIGNALS */

int main(void)
{
	printf("Not supported.\n");
	exit(1);
}

#endif /* _POSIX_REALTIME_SIGNALS */
