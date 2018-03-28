/*
	sleep implementation using alarm
	AUP2, Sec. 9.07.2

	Thanks to Geoff Clare for correcting the original version.

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
/*[slp_handler]*/
static void slp_handler(int signum)
{
}
/*[pgm]*/
unsigned aup_sleep(unsigned secs)
{
	sigset_t set, oset;
	struct sigaction act, oact;
	unsigned prev_alarm, slept, unslept, effective_secs;

	ec_neg1( sigemptyset(&set) )
	ec_neg1( sigaddset(&set, SIGALRM) )
	ec_neg1( sigprocmask(SIG_BLOCK, &set, &oset) )
	prev_alarm = alarm(0);
	if (prev_alarm != 0 && prev_alarm <= secs)
		effective_secs = prev_alarm;
	else {
		memset(&act, 0, sizeof(act));
		act.sa_handler = slp_handler;
		ec_neg1( sigaction(SIGALRM, &act, &oact) )
		effective_secs = secs;
	}
	alarm(effective_secs);
	set = oset;
	ec_neg1( sigdelset(&set, SIGALRM) )
	if (sigsuspend(&set) == -1 && errno != EINTR)
		EC_FAIL
	unslept = alarm(0);
	slept = effective_secs - unslept;
	ec_neg1( sigaction(SIGALRM, &oact, NULL) )
	if (prev_alarm > slept)
		alarm(prev_alarm - slept);
	ec_neg1( sigprocmask(SIG_SETMASK, &oset, NULL) )
	return unslept;

EC_CLEANUP_BGN
	EC_FLUSH("aup_sleep")
	return 0;
EC_CLEANUP_END
}
/*[]*/
static void alarm_handler(int signum)
{
	write(STDOUT_FILENO, "SIGALRM handler\n", 16);
}

int main(void)
{
	struct sigaction act;
	time_t tm;

	memset(&act, 0, sizeof(act));
	act.sa_handler = alarm_handler;
	ec_neg1( sigaction(SIGALRM, &act, NULL) )
	time(&tm);
	printf("Test 1 -- time %ld\n", (long)tm);
	aup_sleep(6);
	time(&tm);
	printf("Test 2 -- time %ld\n", (long)tm);
	alarm(3);
	aup_sleep(6);
	time(&tm);
	printf("Test 3 -- time %ld\n", (long)tm);;
	alarm(9);
	aup_sleep(6);
	time(&tm);
	printf("Sleep over; now pausing -- time %ld\n", (long)tm);
	pause();
	time(&tm);
	printf("Exiting  -- time %ld\n", (long)tm);
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
