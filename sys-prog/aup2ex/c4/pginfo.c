/*
	Display session- and process-group-related information
	AUP2, Sec. 4.03.5

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

/*[showpginfo]*/
#include <termios.h>

static void showpginfo(const char *msg)
{
	int fd;

	printf("%s\n", msg);
	printf("\tprocess ID = %ld; parent = %ld\n",
	  (long)getpid(), (long)getppid());
	printf("\tsession ID = %ld; process-group ID = %ld\n",
	  (long)getsid(0), (long)getpgid(0));
	ec_neg1( fd = open("/dev/tty", O_RDWR) )
	printf("\tcontrolling terminal's foreground process-group ID = %ld\n",
	  (long)tcgetpgrp(fd));
#if _XOPEN_VERSION >= 4
	printf("\tcontrolling-terminal's session ID = %ld\n",
	  (long)tcgetsid(fd));
#else
	printf("\tcontrolling-terminal's session ID = %ld\n",
	  (long)getsid(tcgetpgrp(fd)));
#endif
	ec_neg1( close(fd) )
	return;

EC_CLEANUP_BGN
	EC_FLUSH("showpginfo")
EC_CLEANUP_END
}
/*[catchsig]*/
static void catchsig(int signo)
{
	if (signo == SIGCONT)
		showpginfo("got SIGCONT");
}
/*[main]*/
int main(void)
{
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = catchsig;
	ec_neg1( sigaction(SIGCONT, &act, NULL) )
	showpginfo("initial call");
	while (true)
		sleep(10000);
EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
