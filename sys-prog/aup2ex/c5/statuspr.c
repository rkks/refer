/*
	Display exit status
	AUP2, Sec. 5.08

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
#include "statuspr.h"

/*[display_status]*/
void display_status(pid_t pid, int status)
{
	if (pid != 0)
		printf("Process %ld: ", (long)pid);
	if (WIFEXITED(status))
		printf("Exit value %d\n", WEXITSTATUS(status));
	else {
		char *desc;
		char *signame = get_macrostr("signal", WTERMSIG(status), &desc);
		if (desc[0] == '?')
			desc = signame;
		if (signame[0] == '?')
			printf("Signal #%d", WTERMSIG(status));
		else
			printf("%s", desc);
		if (WCOREDUMP(status))
			printf(" - core dumped");
		if (WIFSTOPPED(status))
			printf(" (stopped)");
#if defined(_XOPEN_UNIX) && !defined(LINUX)
		else if (WIFCONTINUED(status))
			printf(" (continued)");
#endif
		printf("\n");
	}
}
/*[]*/
