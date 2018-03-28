#ifndef lint
static char *RCSid = "$Header$";
#endif

/*
 * logfile.c - ruotines for manipulating the logfile
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
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#include "defs.h"

extern int	errno;
extern int	skipIntervals;

extern char	*logfileName;

static FILE	*logfileFP = NULL;

/*
 * openLogFile - open the log file.
 */
void
openLogfile(int mode)
{
	struct stat st;
	char buf[BUFSIZ];

	if (logfileFP != NULL)
		closeLogfile();

	switch (mode) {
	case O_WRONLY:
		/*
		 * Make sure we're not going to overwrite an existing
		 * logfile, as might happen when they forget the -r
		 * flag.
		 */
		if ((stat(logfileName, &st) == 0) && (st.st_size > 0)) {
			printf("Logfile \"%s\" exists and is non-zero size.\n",
			       logfileName);
			printf("Overwrite [n]? ");

			if (fgets(buf, sizeof(buf), stdin) == NULL) {
				putchar('\n');
				exit(0);
			}

			if (*buf != 'y' && *buf != 'Y')
				exit(0);
		}

		if ((logfileFP = fopen(logfileName, "w")) == NULL)
			fatalError(logfileName, strerror(errno));

		break;
	case O_RDONLY:
		if ((logfileFP = fopen(logfileName, "r")) == NULL)
			fatalError(logfileName, strerror(errno));

		fstat(fileno(logfileFP), &st);

		/*
		 * If they gave the -s option, skip over the proper number
		 * of intervals.
		 */
		if ((skipIntervals * sizeof(IntervalCounters)) > st.st_size)
			fatalError("interval skip count too large", NULL);

		if (skipIntervals > 0) {
			fseek(logfileFP,
			      skipIntervals * sizeof(IntervalCounters), 0);
		}

		break;
	}

}

/*
 * closeLogfile - close the log file.
 */
void
closeLogfile(void)
{
	if (logfileFP != NULL)
		fclose(logfileFP);
}

/*
 * writeLogfileRecord - write an interval counters structure to the log file.
 */
void
writeLogfileRecord(IntervalCounters *counter)
{
	int n;

	if (logfileFP == NULL)
		return;

	n = fwrite((char *) counter, sizeof(IntervalCounters), 1, logfileFP);

	if (n != 1)
		fatalError("fwrite", strerror(errno));

	n = fflush(logfileFP);

	if (n < 0)
		fatalError("fflush", strerror(errno));
}

/*
 * readLogfileRecord - read an interval counters structure from the log file.
 */
int
readLogfileRecord(IntervalCounters *counter)
{
	int n;

	if (logfileFP == NULL)
		return;

	n = fread((char *) counter, sizeof(IntervalCounters), 1, logfileFP);
	return(n);
}
