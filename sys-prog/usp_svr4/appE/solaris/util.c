#ifndef lint
static char *RCSid = "$Header$";
#endif

/*
 * util.c - utility routines for packmon
 *
 * David A. Curry
 * Manager, UNIX Systems Programming Group
 * Purdue University Engineering Computer Network
 * 1285 Electrical Engineering Building
 * West Lafayette, IN 47907-1285 USA
 * davy@ecn.purdue.edu
 *
 * $Log$
 */
#include <sys/param.h>
#include <stdio.h>

#include "defs.h"

/*
 * Usage message printed by usage().
 */
static char usageMessage[] = "\
Usage: %s [-a] [-i interface] [-l logfile] [-n count] [-p portlist]\n\
       %*s [-r] [-s count] [-t seconds] [-v]\n\
  Where:\n\
    -a              print report for all intervals instead of just totals\n\
    -i interface    monitor packets from interface `interface'\n\
    -l logfile      set logfile name to `logfile'\n\
    -n count        process `count' intervals and exit\n\
    -p portlist     read port numbers and service names from `portlist'\n\
    -r              produce a report from an existing logfile\n\
    -s count        skip `count' intervals when reading logfile\n\
    -t seconds      make intervals `seconds' seconds long\n\
    -v              print all source/destination port counts instead of just\n\
                    counts for known ports\n\
";

/*
 * fatalError - print an error message and exit.
 */
void
fatalError(char *str1, char *str2)
{
	extern char *programName;

	fprintf(stderr, "%s: %s", programName, str1);

	if (str2 != NULL)
		fprintf(stderr, ": %s", str2);

	fprintf(stderr, ".\n");

	exit(1);
}

/*
 * usage - print a usage message and exit.
 */
void
usage(void)
{
	extern char *programName;

	fprintf(stderr, usageMessage, programName, strlen(programName), "");
	exit(1);
}
