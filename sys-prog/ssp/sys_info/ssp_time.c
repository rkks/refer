#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <limits.h>
#include "ssp.h"

static void proc_cmd (char *cmd);
static void print_times (clock_t real, struct tms *start, struct tms *end);

int main (int argc, char **argv)
{
	int i;

	for (i = 1; i < argc; i++)
		proc_cmd (argv [i]);

	return (0);
}

static void proc_cmd (char *cmd)
{
	struct tms start_tms;
	struct tms end_tms;
	clock_t start;
	clock_t end;
	int status;

	printf ("Command: %s\n", cmd);

	if ((start = times (&start_tms)) == -1)
		err_msg ("Start times failed");

	if ((status = system (cmd)) == -1)
		err_msg ("system failed");

	if ((end = times (&end_tms)) == -1)
		err_msg ("End times failed");

	print_times (end - start, &start_tms, &end_tms);
	print_term_status (status);
	printf ("\n");
}

static void print_times (clock_t real, struct tms *start, struct tms *end)
{
	static double tps = 0.0;

	if (tps == 0.0)
		if ((tps = (double) sysconf (_SC_CLK_TCK)) == -1)
			err_msg ("sysconf failed");

	printf ("  Real: %7.3f\n", real / tps);
	printf ("  User: %7.3f\n", (end -> tms_utime - start -> tms_utime) / tps);
	printf ("  Sys:  %7.3f\n", (end -> tms_stime - start -> tms_stime) / tps);
	printf ("  Child user: %7.3f\n",
		(end -> tms_cutime - start -> tms_cutime) / tps);
	printf ("  Child sys:  %7.3f\n",
		(end -> tms_cstime - start -> tms_cstime) / tps);
}
