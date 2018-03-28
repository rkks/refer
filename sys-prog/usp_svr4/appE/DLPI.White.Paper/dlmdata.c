/*
 * SYNOPSIS:
 *	dlxmit device ppa localsap destphys destsap n size
 *
 * Open datalink provider.
 * Attach to PPA.
 * Bind to sap
 * Send "n" M_DATA msgs contain "size" bytes of data
 */

/*
typedef unsigned long ulong;
*/

#include	<sys/types.h>
#include	<sys/stropts.h>
#include	<sys/dlpi.h>
#include	<sys/ethernet.h>
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
	int	physlen;
	u_char	phys[ETHERADDRL];
	u_char	addr[MAXDLADDR];
	u_char	localphys[ETHERADDRL];
	long	buf[MAXDLBUF];
	union	DL_primitives	*dlp;
	struct	ether_header	*ehp;
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

	if (physlen != ETHERADDRL)
		err("physlen must be size of ethernet address (6 bytes)");

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
	 * Get our physical address.
	 */
	dlphysaddrreq(fd, DL_CURR_PHYS_ADDR);
	dlphysaddrack(fd, buf);
	dlp = (union DL_primitives*) buf;

	ehp = (struct ether_header*) xmitbuf;
	memcpy(&ehp->ether_dhost, phys, ETHERADDRL);
	memcpy(&ehp->ether_shost, OFFADDR(dlp, dlp->physaddr_ack.dl_addr_offset), ETHERADDRL);
	ehp->ether_type = (u_short) sapval;

	if (strioctl(fd, DLIOCRAW, -1, 0, 0) < 0)
		syserr("ioctl DLIOCRAW");

	/*
	 * Transmit 'size' packet 'n' times.
	 */
	for (i = 0; i < n; i++)
		if (write(fd, xmitbuf, size) < 0)
			syserr("write");

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
