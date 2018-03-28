#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

void print_term_status (int status)
{
	if (WIFEXITED (status))
		printf ("Normal termination; exit status = %d\n",
			WEXITSTATUS (status));
	else if (WIFSIGNALED (status))
		printf ("Abnormal termination; signal = %s%s\n",
			strsignal (WTERMSIG (status)),
			WCOREDUMP (status) ? " (core dumped)" : "");
	else if (WIFSTOPPED (status))
		printf ("Child stopped; signal = %s\n",
			strsignal (WSTOPSIG (status)));
}
