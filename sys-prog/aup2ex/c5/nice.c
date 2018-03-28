/*
	nice command
	AUP2, Sec. 5.15

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

/*[aupnice]*/
#define USAGE "usage: aupnice [-num] command\n"

int main(int argc, char *argv[])
{
	int incr, cmdarg;
	char *cmdname, *cmdpath;

	if (argc < 2) {
		fprintf(stderr, USAGE);
		exit(EXIT_FAILURE);
	}
	if (argv[1][0] == '-') {
		incr = atoi(&argv[1][1]);
		cmdarg = 2;
	}
	else {
		incr = 10;
		cmdarg = 1;
	}
	if (cmdarg >= argc) {
		fprintf(stderr, USAGE);
		exit(EXIT_FAILURE);
	}
	(void)nice(incr);
	cmdname = strchr(argv[cmdarg], '/');
	if (cmdname == NULL)
		cmdname = argv[cmdarg];
	else
		cmdname++;
	cmdpath = argv[cmdarg];
	argv[cmdarg] = cmdname;
	execvp(cmdpath, &argv[cmdarg]);
	EC_FAIL

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
