/*
 * NAME
 *  dlrcv - read and display raw datalink packets
 * SYNOPSIS:
 *	dlrcv [ -bp ] device ppa sap
 *
 * Open datalink provider.
 * Attach to PPA.
 * Bind to sap
 * Issue DLIOCRAW ioctl.
 * Read and print messages
 */

/*
typedef	unsigned long ulong;
*/

#include	<sys/types.h>
#include	<sys/time.h>
#include	<sys/stream.h>
#include	<sys/stropts.h>
#include	<sys/dlpi.h>
#include	<sys/bufmod.h>
#include	<stdio.h>
#include	"dltest.h"

long	databuf[MAXDLBUF];

extern	char	*optarg;
extern	int	optind;

int	promisc = 0;	/* promiscuous mode "off" by default */
int	bufmod = 0;		/* push buffer module, "off" by default */

main(ac, av)
int	ac;
char	*av[];
{
	long	buf[MAXDLBUF];
	char	*device;
	int	ppa;
	int	fd;
	int	sap;
	struct	strbuf	data;
	int	flags;
	int	i;
	int	c;
	int	offset;
	int	len;
	struct	timeval	t;
	u_int	chunksize = 16 * 1024;
	struct	sb_hdr	*bp;
	char	*p, *limp;

	if (ac < 4)
		usage(av[0]);
	while ((c = getopt(ac, av, "bp")) != -1)
		switch (c) {
			case 'b':	/* push buffer module */
				bufmod = 1;
				break;

			case 'p':	/* promiscuous mode "on" */
				promisc = 1;
				break;

			default:
				usage(av[0]);
				break;
		}

	if ((ac - optind) != 3)
		usage(av[0]);

	device = av[optind++];
	ppa = atoi(av[optind++]);
	sap	= atoi(av[optind++]);

	/*
	 * Validate arguments.
	 */
	if (ppa < 0)
		err("ppa cannot be negative");
	if (sap < 0)
		err("sap cannot be negative");

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
	 * Optionally enable promiscuous mode.
	 */
	if (promisc) {
		dlpromisconreq(fd, DL_PROMISC_PHYS);
		dlokack(fd, buf);
	}

	/*
	 * Bind.
	 */
	dlbindreq(fd, sap, 0, DL_CLDLS, 0, 0);
	dlbindack(fd, buf);

	/*
	 * Issue DLIOCRAW ioctl.
	 */
	if (strioctl(fd, DLIOCRAW, -1, 0, NULL) < 0)
		syserr("DLIOCRAW");

	/*
	 * Push and configure buffer module.
	 */
	if (bufmod) {
		if (ioctl(fd, I_PUSH, "bufmod") < 0)
			syserr("push bufmod");

		t.tv_sec = 0;
		t.tv_usec = 500000;	/* 0.5s */
		if (strioctl(fd, SBIOCSTIME, -1, sizeof (struct timeval),
			&t) < 0)
			syserr("SBIOCSTIME");
		if (strioctl(fd, SBIOCSCHUNK, -1, sizeof (u_int),
			&chunksize) < 0)
			syserr("SBIOCSCHUNK");
	}

	/*
	 * Flush the read side of the Stream.
	 */
	if (ioctl(fd, I_FLUSH, FLUSHR) < 0)
		syserr("I_FLUSH");

	/*
	 * Read packets.
	 */

	data.buf = (char *) databuf;
	data.maxlen = MAXDLBUF;
	data.len = 0;

	while (getmsg(fd, NULL, &data, &flags) == 0) {

		limp = p + data.len;

		if (data.len && bufmod) {
			for (p = data.buf; p < limp; p += bp-> sbh_totlen) {
				bp = (struct sb_hdr*) p;
				for (i = 0; i < bp->sbh_msglen; i++)
					printf("%02x ", *(p +
						sizeof (struct sb_hdr) + i)
						& 0xff);
				printf("\n");
			}
		} else if (data.len) {
			for (i = 0; i < data.len; i++)
				printf("%02x ", data.buf[i] & 0xff);
			printf("\n");
		}

		data.len = 0;
	}
}

usage(av0)
char	*av0;
{
	(void) fprintf(stderr, "%s:  ", av0);
	(void) fprintf(stderr, "Usage:  %s [ -bp ] device ppa sap\n", av0);
	(void) exit(1);
}
