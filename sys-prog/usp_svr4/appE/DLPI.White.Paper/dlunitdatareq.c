/*
 * SYNOPSIS:
 *	dlxmit device ppa localsap destphys destsap n size
 *
 * Open datalink provider.
 * Attach to PPA.
 * Bind to sap
 * Send "n" DL_UNITDATA_REQ msgs contain "size" bytes of data
 */

/*
typedef unsigned long ulong;
*/

#include	<sys/types.h>
#include	<sys/stropts.h>
#include	<sys/dlpi.h>
#include	<stdio.h>
#include	"dltest.h"

#define	ABS(x)		((x) < 0 ? -(x) : (x))
#define	BITSPERBYTE	(8)

/*
 * Xmit data buffer.  Fill with whatever pattern you like ...
 */
char	xmitbuf[MAXDLBUF];

main(ac, av)
int	ac;
char	*av[];
{
	char	*device;
	int	ppa;
	int	fd;
	int	localsap;
	int	sapval;
	int	n;
	int	size;
	int	physlen, saplen;
	u_char	phys[MAXDLADDR], sap[MAXDLADDR];
	u_char	addr[MAXDLADDR];
	int	addrlen;
	long	buf[MAXDLBUF];
	union	DL_primitives	*dlp;
	int	i;


	/* initialize buf[] */
	for (i = 0; i < MAXDLBUF; i++)
		buf[i] = (unsigned char) i & 0xff;

	if (ac != 8)
		usage(av[0]);

	device = av[1];
	ppa = atoi(av[2]);
	localsap	= atoi(av[3]);
	n = atoi(av[6]);
	size = atoi(av[7]);

	/*
	 * Validate arguments.
	 */
	if (ppa < 0)
		err("ppa cannot be negative");
	if (localsap < 0)
		err("local sap cannot be negative");
	if (n < 0)
		err("n cannot be negative");
	if (size > MAXDLBUF)
		err("size must be less than or equal to %d", MAXDLBUF);

	/*
	 * Convert destination address string to address.
	 */
	physlen = stringtoaddr(av[4], phys);
	sapval = atoi(av[5]);
	for (i = 0; i < sizeof (long); i++)
		sap[i] = (sapval >> (i * BITSPERBYTE)) & 0xff;

	/*
	 * Open the device.
	 */
	if ((fd = open(device, 2)) < 0)
		syserr(device);

	/*
	 * Attach.
	 */
	dlattachreq(fd, ppa);
	dlokack(fd, buf);

	/*
	 * Bind.
	 */
	dlbindreq(fd, localsap, 0, DL_CLDLS, 0, 0);
	dlbindack(fd, buf);

	/*
	 * Get info.
	 */
	dlinforeq(fd);
	dlinfoack(fd, buf);

	/*
	 * Verify sap and phys address lengths.
	 */

	dlp = (union DL_primitives*) buf;

	saplen = ABS(dlp->info_ack.dl_sap_length);
	if (physlen != (dlp->info_ack.dl_addr_length - saplen))
		err("invalid destination physical address length");
	addrlen = saplen + physlen;

	/*
	 * Construct destination address.
	 */
	if (dlp->info_ack.dl_sap_length > 0) {	/* order is sap+phys */
		(void) memcpy((char*)addr, (char*)sap, saplen);
		(void) memcpy((char*)addr+saplen, (char*)phys, physlen);
	} else {	/* order is phys+sap */
		(void) memcpy((char*)addr, (char*)phys, physlen);
		(void) memcpy((char*)addr+physlen, (char*)sap, saplen);
	}

	/*
	 * Transmit 'size' packet 'n' times.
	 */
	for (i = 0; i < n; i++)
		dlunitdatareq(fd, addr, addrlen, 0, 0, xmitbuf, size);

	return (0);
}

usage(av0)
char	*av0;
{
	(void) fprintf(stderr, "%s:  ", av0);
	(void) fprintf(stderr,
		"Usage:  %s device ppa localsap destphys destsap n size\n", av0);
	(void) exit(1);
}
