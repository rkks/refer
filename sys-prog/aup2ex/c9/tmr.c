/*
	Timer and clock examples
	AUP2, Sec. 9.07.4, 9.07.5

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

/*
	Linux defines _POSIX_TIMERS as > 0, but doesn't seem to support
	the feature. The headers seem to be OK, but there is no man page.
	If you figure this out, please send mail to aup@basepath.com.
*/

#if _POSIX_TIMERS > 0 && !defined(LINUX)

#include <sys/time.h>

/*[handler]*/
void handler(int signum)
{
	write(STDOUT_FILENO, "\nX\n", 3);
}
/*[timer_try1]*/
void timer_try1(void)
{
	struct sigaction act;
	struct itimerval itv;
	char buf[100];
	ssize_t nread;

	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	ec_neg1( sigaction(SIGALRM, &act, NULL) )
	memset(&itv, 0, sizeof(itv));
	itv.it_interval.tv_sec = 2;
	itv.it_value.tv_sec = 2;
	ec_neg1( setitimer(ITIMER_REAL, &itv, NULL) )
	while (true) {
		switch( nread = read(STDIN_FILENO, buf, sizeof(buf) - 1) ) {
		case -1:
			EC_FAIL
		case 0:
			printf("EOF\n");
			break;
		default:
			if (nread > 0)
				buf[nread] = '\0';
			ec_neg1( write(STDOUT_FILENO, buf, strlen(buf)) )
			continue;
		}
		break;
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("timer_try1")
EC_CLEANUP_END
}

void timer_try2(void)
{
	struct sigaction act;
	struct itimerval itv;
	char buf[100];
	ssize_t nread;

/*[timer_try2]*/
	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	act.sa_flags = SA_RESTART;
	ec_neg1( sigaction(SIGALRM, &act, NULL) )
/*[]*/
	memset(&itv, 0, sizeof(itv));
	itv.it_interval.tv_sec = 2;
	itv.it_value.tv_sec = 2;
	ec_neg1( setitimer(ITIMER_REAL, &itv, NULL) )
	while (true) {
		switch( nread = read(STDIN_FILENO, buf, sizeof(buf) - 1) ) {
		case -1:
			EC_FAIL
		case 0:
			printf("EOF\n");
			break;
		default:
			if (nread > 0)
				buf[nread] = '\0';
			ec_neg1( write(STDOUT_FILENO, buf, strlen(buf)) )
			continue;
		}
		break;
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("timer_try2")
EC_CLEANUP_END
}
/*[clocks]*/
void clocks(void)
{
	struct timespec ts;
	time_t tm;

	ec_neg1( time(&tm) )
	printf("time() Time: %ld secs.\n", (long)tm);
	printf("CLOCK_REALTIME:\n");
	ec_neg1( clock_gettime(CLOCK_REALTIME, &ts) )
	printf("Time: %ld.%09ld secs.\n", (long)ts.tv_sec, (long)ts.tv_nsec);
	ec_neg1( clock_getres(CLOCK_REALTIME, &ts) )
	printf("Res.: %ld.%09ld secs.\n", (long)ts.tv_sec, (long)ts.tv_nsec);
	return;

EC_CLEANUP_BGN
	EC_FLUSH("clocks")
EC_CLEANUP_END
}
/*[]*/
int main(void)
{
	clocks();
	exit(EXIT_SUCCESS);
}

#else /* _POSIX_TIMERS */

int main(void)
{
	printf("Not supported.\n");
	exit(1);
}

#endif /* _POSIX_TIMERS */
