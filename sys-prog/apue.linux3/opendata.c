#include	<stdio.h>

#define	DATAFILE	"datafile"

FILE *
open_data(void)
{
	FILE	*fp;
	char	databuf[BUFSIZ];  /* setvbuf makes this the stdio buffer */

	if ( (fp = fopen(DATAFILE, "r")) == NULL)
		return(NULL);

	if (setvbuf(fp, databuf, BUFSIZ, _IOLBF) != 0)
		return(NULL);

	return(fp);		/* error */
}
