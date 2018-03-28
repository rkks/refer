#include	<stdio.h>

/* th extern char	*sys_errlist[]; */
/*th extern int	sys_nerr; */

const char *const	/* th */
strerror(int error)
{
	static char	mesg[30];

	if (error >= 0 && error <= sys_nerr)
		return(sys_errlist[error]);

	sprintf(mesg, "Unknown error (%d)", error);
	return(mesg);
}
