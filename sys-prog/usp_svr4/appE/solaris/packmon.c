#ifndef lint
static char *RCSid = "$Header$";
#endif

/*
 * packmon - packet monitoring and traffic classification
 *
 * Usage: packmon [-a] [-i interface] [-l logfile] [-n count] [-p portlist]
 *		  [-r] [-s count] [-t seconds] [-v]
 *
 *	-a		print report for all intervals instead of just totals
 *	-i interface	monitor packets from interface "interface"
 *	-l logfile	set logfile name to "logfile"
 *	-n count	process "count" intervals and exit
 *	-p portlist	read port numbers and service names from "portlist"
 *	-r		produce a report from an existing logfile
 *	-s count	skip "count" intervals when reading logfile
 *	-t seconds	make intervals "seconds" seconds long
 *	-v		print all source/destination port counts instead of
 *			just counts for known ports
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
#include <string.h>
#include <stdio.h>

#include "defs.h"

int	maxIntervals = 0;		/* -n count			*/
int	skipIntervals = 0;		/* -s count			*/
int	intervalLength = 0;		/* -t seconds			*/

int	allPorts = FALSE;		/* -v				*/
int	allIntervals = FALSE;		/* -a				*/
int	generateReport = FALSE;		/* -r				*/

char	*programName = NULL;		/* argv[0]			*/
char	*logfileName = NULL;		/* -l logfile			*/
char	*portlistName = NULL;		/* -p portlist			*/
char	*interfaceName = NULL;		/* -i interface			*/

u_long	networkMask;			/* network mask (Class A,B,C,D)	*/
u_long	networkAddress;			/* network addr (Class A,B,C,D)	*/
u_long	subNetworkMask;			/* subnetwork mask		*/
u_long	subNetworkAddress;		/* subnetwork address		*/

int
main(int argc, char **argv)
{
	int opt;
	extern char *optarg;
	extern int optind, opterr;

	opterr = 0;
	logfileName = DEFAULT_LOGFILE;
	portlistName = DEFAULT_PORTLIST;
	intervalLength = DEFAULT_INTERVAL;

	/*
	 * Save program name.
	 */
	if ((programName = strrchr(*argv, '/')) == NULL)
		programName = *argv;
	else
		programName++;

	/*
	 * Process arguments.
	 */
	while ((opt = getopt(argc, argv, "ai:l:n:p:rs:t:v")) != EOF) {
		switch (opt) {
		case 'a':
			allIntervals = TRUE;
			break;
		case 'i':
			interfaceName = optarg;
			break;
		case 'l':
			logfileName = optarg;
			break;
		case 'n':
			maxIntervals = atoi(optarg);
			break;
		case 'p':
			portlistName = optarg;
			break;
		case 'r':
			generateReport = TRUE;
			break;
		case 's':
			skipIntervals = atoi(optarg);
			break;
		case 't':
			intervalLength = atoi(optarg);
			break;
		case 'v':
			allPorts = TRUE;
			break;
		case '?':
			usage();
			break;
		}
	}

	if (optind != argc)
		usage();

	/*
	 * Figure out what to do.
	 */
	if (generateReport == TRUE)
		reportGenerator();
	else
		packetMonitor();

	exit(0);
}
