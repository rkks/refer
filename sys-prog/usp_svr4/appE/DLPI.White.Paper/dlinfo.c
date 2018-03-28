/*
 * NAME
 *  dlinfo - print information about the datalink provider
 * SYNOPSIS:
 *	dlinfo device ppa sap
 *
 * Open datalink provider.
 * Attach to PPA.
 * Bind to sap
 * Get DL_INFO_ACK.
 * Print DL_INFO_ACK information.
 */

#include	<sys/types.h>
#include	<sys/stropts.h>
#include	<sys/dlpi.h>
#include	<sys/signal.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	"dltest.h"

main(ac, av)
int	ac;
char	*av[];
{
	long	buf[MAXDLBUF];	/* aligned on long */
	union	DL_primitives	*dlp;
	char	*device;
	int	ppa;
	int	fd;
	int	sap;

	dlp = (union DL_primitives*) buf;

	if (ac != 4)
		usage(av[0]);

	device = av[1];
	ppa = atoi(av[2]);
	sap	= atoi(av[3]);

	/*
	 * Open the device.
	 */
	if ((fd = open(device, O_RDWR)) < 0)
		syserr(device);

	/*
	 * Attach.
	 */
	dlattachreq(fd, ppa);
	dlokack(fd, buf);

	/*
	 * Bind.
	 */
	dlbindreq(fd, sap, 0, DL_CLDLS, 0, 0);
	dlbindack(fd, buf);

	/*
	 * Issue DL_INFO_REQ.
	 */
	dlinforeq(fd);
	dlinfoack(fd, buf);

	/*
	 * Print DL_INFO_ACK values.
	 */
	printdlprim(dlp);

	return (0);
}

usage(av0)
char	*av0;
{
	(void) fprintf(stderr, "%s:  ", av0);
	(void) fprintf(stderr, "Usage:  %s device ppa sap\n", av0);
	(void) exit(1);
}
