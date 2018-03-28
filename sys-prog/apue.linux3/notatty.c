#include	<stdio.h>

/*
 * The following prints errno=25 (ENOTTY) under 4.3BSD and SVR2,
 * when stdout is redirected to a file.
 * Under SVR4 and 44BSD it works OK.
 */

int
main()
{
	int			fd;
	extern int	errno;

	if ( (fd = open("/no/such/file", 0)) < 0) {
		printf("open error: ");
		printf("errno = %d\n", errno);
	}
	exit(0);
}
