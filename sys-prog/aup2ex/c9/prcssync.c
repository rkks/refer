/*
	Process-synchronization examples
	AUP2, Sec. 9.02.3

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
/*[try1]*/
void try1(void)
{
	if (fork() == 0) {
		printf("child\n");
		exit(EXIT_SUCCESS);
	}
	printf("parent\n");
	return;
}
/*[]*/
// SIGUSR1 arriving before handler is installed will kill process
/*[try2]*/
static volatile sig_atomic_t got_sig;

static void handler(int signum)
{
	if (signum == SIGUSR1)
		got_sig = 1;
}

void try2(void)
{
	pid_t pid;

	got_sig = 0;
	ec_neg1( pid = fork() )
	if (pid == 0) {
		struct sigaction act;

		memset(&act, 0, sizeof(act));
		act.sa_handler = handler;
		ec_neg1( sigaction(SIGUSR1, &act, NULL) )
		while (got_sig == 0)
			if (pause() == -1 && errno != EINTR)
				EC_FAIL
		printf("child\n");
		exit(EXIT_SUCCESS);
	}
	printf("parent\n");
	ec_neg1( kill(pid, SIGUSR1) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("try2")
EC_CLEANUP_END
}
/*[]*/
// fixed: SIGUSR1 arriving before handler is installed will kill process
// arriving between while and pause will cause it to be lost
/*[try3]*/
void try3(void)
{
	sigset_t set;
	pid_t pid;

	got_sig = 0;
	ec_neg1( sigemptyset(&set) )
	ec_neg1( sigaddset(&set, SIGUSR1) )
	ec_neg1( sigprocmask(SIG_SETMASK, &set, NULL) )
	ec_neg1( pid = fork() )
	if (pid == 0) {
		struct sigaction act;
		sigset_t suspendset;

		memset(&act, 0, sizeof(act));
		act.sa_handler = handler;
		ec_neg1( sigaction(SIGUSR1, &act, NULL) )
		ec_neg1( sigfillset(&suspendset) )
		ec_neg1( sigdelset(&suspendset, SIGUSR1) )
		ec_neg1( sigprocmask(SIG_SETMASK, &suspendset, NULL) )
		while (got_sig == 0)
			if (pause() == -1 && errno != EINTR)
				EC_FAIL
		printf("child\n");
		exit(EXIT_SUCCESS);
	}
	printf("parent\n");
	ec_neg1( kill(pid, SIGUSR1) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("try3")
EC_CLEANUP_END
}
/*[try4]*/
void try4(void)
{
	sigset_t set;
	pid_t pid;

	got_sig = 0;
	ec_neg1( sigemptyset(&set) )
	ec_neg1( sigaddset(&set, SIGUSR1) )
	ec_neg1( sigprocmask(SIG_SETMASK, &set, NULL) )
	ec_neg1( pid = fork() )
	if (pid == 0) {
		struct sigaction act;
		sigset_t suspendset;

		memset(&act, 0, sizeof(act));
		act.sa_handler = handler;
		ec_neg1( sigaction(SIGUSR1, &act, NULL) )
		ec_neg1( sigfillset(&suspendset) )
		ec_neg1( sigdelset(&suspendset, SIGUSR1) )
		if (sigsuspend(&suspendset) == -1 && errno != EINTR)
			EC_FAIL
		printf("child\n");
		exit(EXIT_SUCCESS);
	}
	printf("parent\n");
	ec_neg1( kill(pid, SIGUSR1) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("try4")
EC_CLEANUP_END
}

/*
	FreeBSD defines sigwait only if the pthread libraries are included,
	but then sigaction seems to fail with EINVAL. If you manage to sort
	this out, please send email to aup#basepath.com.
*/
#ifndef FREEBSD
/*[try5]*/
void try5(void)
{
	sigset_t set;
	pid_t pid;

	ec_neg1( sigemptyset(&set) )
	ec_neg1( sigaddset(&set, SIGUSR1) )
	ec_neg1( sigprocmask(SIG_SETMASK, &set, NULL) )
	ec_neg1( pid = fork() )
	if (pid == 0) {
		int signum;

		ec_rv( sigwait(&set, &signum) )
		printf("child\n");
		exit(EXIT_SUCCESS);
	}
	printf("parent\n");
	ec_neg1( kill(pid, SIGUSR1) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("try5")
EC_CLEANUP_END
}
/*[]*/
#endif /* FREEBSD */
/*
	Same thing, but with a pipe.
*/
/*[try6]*/
void try6(void)
{
	int pfd[2];
	pid_t pid;

	ec_neg1( pipe(pfd) )
	ec_neg1( pid = fork() )
	if (pid == 0) {
		char c;

		ec_neg1( close(pfd[1]) )
		ec_neg1( read(pfd[0], &c, 1) )
		ec_neg1( close(pfd[0]) )
		printf("child\n");
		exit(EXIT_SUCCESS);
	}
	printf("parent\n");
	ec_neg1( close(pfd[0]) )
	ec_neg1( close(pfd[1]) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("try6")
EC_CLEANUP_END
}
/*[]*/
int main(void)
{
	try1();
	sleep(2);
	try2();
	sleep(2);
	try3();
	sleep(2);
	try4();
	sleep(2);
#ifndef FREEBSD
	try5();
	sleep(2);
#endif /* FREEBSD */
	try6();
	sleep(2);
	exit(EXIT_SUCCESS);
}
