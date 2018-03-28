/*
	alarm example
	AUP2, Sec. 8.07.1

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
/*[pgm]*/
static void handler(int signum)
{
}

int main(void)
{
	struct sigaction act;
	char buf[100];
	ssize_t rtn;

	memset(&act, 0, sizeof(act));
	act.sa_handler = handler;
	ec_neg1( sigaction(SIGALRM, &act, NULL) )
	alarm(5);
	if ((rtn = read(STDIN_FILENO, buf, sizeof(buf) - 1)) == -1) {
		if (errno == EINTR)
			printf("Timed out... type faster next time!\n");
		else
			EC_FAIL
	}
	alarm(0);
	if (rtn == 0)
		printf("Got EOF\n");
	else if (rtn > 0) {
		buf[rtn] = '\0';
		printf("Got %s", buf);
	}
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
