#ifndef lint
static char *RCSid = "$Header$";
#endif

/*
 * dlpi.c - routines for working with the dlpi interface
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
#include <sys/stropts.h>
#include <sys/stream.h>
#include <sys/dlpi.h>
#include <sys/bufmod.h>
#include <sys/socket.h>
#include <sys/sockio.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <net/if.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

#include "defs.h"

extern int	errno;

extern char	*interfaceName;

extern u_long	networkMask;
extern u_long	networkAddress;
extern u_long	subNetworkMask;
extern u_long	subNetworkAddress;

static void	sigalrm(int);
static void	dlpiInfoRequest(int);
static int	dlpiOkAck(int, char *);
static int	dlpiInfoAck(int, char *);
static int	dlpiBindAck(int, char *);
static void	dlpiAttachRequest(int, u_long);
static void	dlpiPromiscRequest(int, u_long);
static int	dlpiExpecting(int, union DL_primitives *);
static void	dlpiBindRequest(int, u_long, u_long, u_long, u_long, u_long);
static int	streamGetMsg(int, struct strbuf *, struct strbuf *, int *, char *);

/*
 * dlpiInitialize - initalize a DLPI session.
 */
int dlpiInitialize(void)
{
	char *p;
	u_int chunksz;
	struct ifreq ifr;
	struct ifconf ifc;
	char cbuf[BUFSIZ];
	struct ifreq *ifrp;
	struct strioctl si;
	char devname[BUFSIZ];
	struct timeval timeout;
	long buf[DLPI_MAXDLBUF];
	int n, s, fd, devppa, truncation;
	struct sockaddr_in sinaddr, sinmask;

	/*
	 * Grab a socket.
	 */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		fatalError("socket", strerror(errno));

	/*
	 * If the interface device was not specified,
	 * get the default one.
	 */
	if (interfaceName == NULL) {
		ifc.ifc_buf = cbuf;
		ifc.ifc_len = sizeof(cbuf);

		/*
		 * See what devices we've got.
		 */
		if (ioctl(s, SIOCGIFCONF, (char *) &ifc) < 0)
			fatalError("ioctl: SIOCGIFCONF", strerror(errno));

		/*
		 * Take the first device we encounter.
		 */
		ifrp = ifc.ifc_req;
		for (n = ifc.ifc_len/sizeof(struct ifreq); n > 0; n--,ifrp++) {
			/*
			 * Skip the loopback interface.
			 */
			if (strcmp(ifrp->ifr_name, "lo0") == 0)
				continue;

			interfaceName = strdup(ifrp->ifr_name);
			break;
		}
	}

	/*
	 * Get the interface's network address.
	 */
	ifrp = &ifr;
	strcpy(ifrp->ifr_name, interfaceName);

	if (ioctl(s, SIOCGIFADDR, (caddr_t) ifrp) < 0)
		fatalError("ioctl: SIOCGIFADDR", strerror(errno));

	memcpy((char *) &sinaddr, (char *) &ifrp->ifr_addr,
		sizeof(struct sockaddr_in));

	/*
	 * Get the interface's network mask.
	 */
	ifrp = &ifr;
	strcpy(ifrp->ifr_name, interfaceName);

	if (ioctl(s, SIOCGIFNETMASK, (caddr_t) ifrp) < 0)
		fatalError("ioctl: SIOCGIFNETMASK", strerror(errno));

	memcpy((char *) &sinmask, (char *) &ifrp->ifr_addr,
		sizeof(struct sockaddr_in));

	(void) close(s);

	/*
	 * Now turn this information into the network
	 * and subnetwork addresses and masks.
	 */
	networkAddress = subNetworkAddress = ntohl(sinaddr.sin_addr.s_addr);
	subNetworkMask = ntohl(sinmask.sin_addr.s_addr);

	if (IN_CLASSA(networkAddress))
		networkMask = IN_CLASSA_NET;
	else if (IN_CLASSB(networkAddress))
		networkMask = IN_CLASSB_NET;
	else
		networkMask = IN_CLASSC_NET;

	networkAddress = networkAddress & networkMask;
	subNetworkAddress = subNetworkAddress & subNetworkMask;

	/*
	 * Split the device name into type and unit number.
	 */
	if ((p = strpbrk(interfaceName, "0123456789")) == NULL)
		return(-1);

	strcpy(devname, DLPI_DEVDIR);
	strncat(devname, interfaceName, p - interfaceName);
	devppa = atoi(p);

	/*
	 * Open the device.
	 */
	if ((fd = open(devname, O_RDWR)) < 0) {
		if (errno == ENOENT || errno == ENXIO)
			return(-1);

		fatalError(devname, strerror(errno));
	}

	/*
	 * Attach to the device.  If this fails, the device
	 * does not exist.
	 */
	dlpiAttachRequest(fd, devppa);

	if (dlpiOkAck(fd, (char *) buf) < 0) {
		close(fd);
		return(-1);
	}

	/*
	 * Bind to the specific unit.
	 */
	dlpiBindRequest(fd, 0, 0, DL_CLDLS, 0, 0);

	if (dlpiBindAck(fd, (char *) buf) < 0)
		fatalError("dlpiBindAck failed", NULL);

	/*
	 * Set promiscuous mode.
	 */
	dlpiPromiscRequest(fd, DL_PROMISC_PHYS);

	if (dlpiOkAck(fd, (char *) buf) < 0)
		fatalError("DL_PROMISC_PHYS failed", NULL);

	/*
	 * We want all types of packets.
	 */
	dlpiPromiscRequest(fd, DL_PROMISC_SAP);

	if (dlpiOkAck(fd, (char *) buf) < 0)
		fatalError("DL_PROMISC_SAP failed", NULL);

	/*
	 * We want raw packets with the packet frame header.
	 */
	si.ic_cmd = DLIOCRAW;
	si.ic_timout = -1;
	si.ic_len = 0;
	si.ic_dp = 0;

	if (ioctl(fd, I_STR, &si) < 0)
		fatalError("ioctl: I_STR DLIOCRAW", strerror(errno));

	/*
	 * Push and configure the streams buffering module.
	 */
	if (ioctl(fd, I_PUSH, DLPI_BUFMOD) < 0)
		fatalError("ioctl: I_PUSH BUFMOD", NULL);

	/*
	 * Set the read timeout.
	 */
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	si.ic_timout = -1;
	si.ic_cmd = SBIOCSTIME;
	si.ic_len = sizeof(timeout);
	si.ic_dp = (char *) &timeout;

	if (ioctl(fd, I_STR, (char *) &si) < 0)
		fatalError("ioctl: I_STR SBIOCSTIME", NULL);

	/*
	 * Set the chunk size.
	 */
	chunksz = DLPI_CHUNKSIZE;

	si.ic_timout = -1;
	si.ic_cmd = SBIOCSCHUNK;
	si.ic_len = sizeof(chunksz);
	si.ic_dp = (char *) &chunksz;

	if (ioctl(fd, I_STR, (char *) &si) < 0)
		fatalError("ioctl: I_STR SBIOCSCHUNK", NULL);

	/*
	 * Set snapshot mode.
	 */
	truncation = DLPI_TRUNCATE;

	si.ic_timout = -1;
	si.ic_cmd = SBIOCSSNAP;
	si.ic_len = sizeof(truncation);
	si.ic_dp = (char *) &truncation;

	if (ioctl(fd, I_STR, (char *) &si) < 0)
		fatalError("ioctl: I_STR SBIOCSSNAP", NULL);

	return(fd);
}

/*
 * dlpiFlush - flush packets we're not interested in.
 */
void
dlpiFlush(int fd)
{
	if (ioctl(fd, I_FLUSH, FLUSHR) < 0)
		fatalError("ioctl: I_FLUSH", NULL);
}

/*
 * dlpiInfoRequest - request information about the data link provider.
 */
static void
dlpiInfoRequest(int fd)
{
	dl_info_req_t info_req;
	struct strbuf ctl;
	int flags;

	info_req.dl_primitive = DL_INFO_REQ;

	ctl.maxlen = 0;
	ctl.len = sizeof(info_req);
	ctl.buf = (char *) &info_req;

	flags = RS_HIPRI;

	if (putmsg(fd, &ctl, (struct strbuf *) NULL, flags) < 0)
		fatalError("dlpiInfoRequest: putmsg", strerror(errno));
}

/*
 * dlpiAttachRequest - send a request to attach.
 */
static void
dlpiAttachRequest(int fd, u_long ppa)
{
	dl_attach_req_t	attach_req;
	struct strbuf ctl;
	int flags;

	attach_req.dl_primitive = DL_ATTACH_REQ;
	attach_req.dl_ppa = ppa;

	ctl.maxlen = 0;
	ctl.len = sizeof(attach_req);
	ctl.buf = (char *) &attach_req;

	flags = 0;

	if (putmsg(fd, &ctl, (struct strbuf *) NULL, flags) < 0)
		fatalError("dlpiAttachRequest: putmsg", strerror(errno));
}

/*
 * dlpiPromiscRequest - send a request to turn promiscuous mode on.
 */
static void
dlpiPromiscRequest(int fd, u_long level)
{
	dl_promiscon_req_t promiscon_req;
	struct strbuf ctl;
	int flags;

	promiscon_req.dl_primitive = DL_PROMISCON_REQ;
	promiscon_req.dl_level = level;

	ctl.maxlen = 0;
	ctl.len = sizeof(promiscon_req);
	ctl.buf = (char *) &promiscon_req;

	flags = 0;

	if (putmsg(fd, &ctl, (struct strbuf *) NULL, flags) < 0)
		fatalError("dlpiPromiscRequest: putmsg", strerror(errno));
}

/*
 * dlpiBindRequest - send a request to bind.
 */
static void
dlpiBindRequest(int fd, u_long sap, u_long max_conind, u_long service_mode,
	  u_long conn_mgmt, u_long xidtest)
{
	dl_bind_req_t bind_req;
	struct strbuf ctl;
	int flags;

	bind_req.dl_primitive = DL_BIND_REQ;
	bind_req.dl_sap = sap;
	bind_req.dl_max_conind = max_conind;
	bind_req.dl_service_mode = service_mode;
	bind_req.dl_conn_mgmt = conn_mgmt;

	/*
	 * DLPI 2.0 only?
	 */
	bind_req.dl_xidtest_flg = xidtest;

	ctl.maxlen = 0;
	ctl.len = sizeof(bind_req);
	ctl.buf = (char *) &bind_req;

	flags = 0;

	if (putmsg(fd, &ctl, (struct strbuf *) NULL, flags) < 0)
		fatalError("dlpiBindRequest: putmsg", strerror(errno));
}

/*
 * dlpiOkAck - general acknowledgement reception.
 */
static int
dlpiOkAck(int fd, char *bufp)
{
	union DL_primitives *dlp;
	struct strbuf ctl;
	int flags;

	ctl.maxlen = DLPI_MAXDLBUF;
	ctl.len = 0;
	ctl.buf = bufp;

	if (streamGetMsg(fd, &ctl, (struct strbuf *) NULL, &flags,
			 "dlpiOkAck") < 0)
		return(-1);

	dlp = (union DL_primitives *) ctl.buf;

	if (dlpiExpecting(DL_OK_ACK, dlp) < 0)
		return(-1);

	if (ctl.len < sizeof(dl_ok_ack_t))
		return(-1);

	if (flags != RS_HIPRI)
		return(-1);

	if (ctl.len < sizeof(dl_ok_ack_t))
		return(-1);

	return(0);
}

/*
 * dlpiInfoAck - receive an ack to a dlpiInfoRequest.
 */
static int
dlpiInfoAck(int fd, char *bufp)
{
	union DL_primitives *dlp;
	struct strbuf ctl;
	int flags;

	ctl.maxlen = DLPI_MAXDLBUF;
	ctl.len = 0;
	ctl.buf = bufp;

	if (streamGetMsg(fd, &ctl, (struct strbuf *) NULL, &flags,
			 "dlpiInfoAck") < 0)
		return(-1);

	dlp = (union DL_primitives *) ctl.buf;

	if (dlpiExpecting(DL_INFO_ACK, dlp) < 0)
		return(-1);

	if (ctl.len < sizeof(dl_info_ack_t))
		return(-1);

	if (flags != RS_HIPRI)
		return(-1);

	if (ctl.len < sizeof(dl_info_ack_t))
		return(-1);

	return(0);
}

/*
 * dlpiBindAck - receive an ack to a dlpiBindRequest.
 */
static int
dlpiBindAck(int fd, char *bufp)
{
	union DL_primitives *dlp;
	struct strbuf ctl;
	int flags;

	ctl.maxlen = DLPI_MAXDLBUF;
	ctl.len = 0;
	ctl.buf = bufp;

	if (streamGetMsg(fd, &ctl, (struct strbuf *) NULL, &flags,
			 "dlpiBindAck") < 0)
		return(-1);

	dlp = (union DL_primitives *) ctl.buf;

	if (dlpiExpecting(DL_BIND_ACK, dlp) < 0)
		return(-1);

	if (flags != RS_HIPRI)
		return(-1);

	if (ctl.len < sizeof(dl_bind_ack_t))
		return(-1);

	return(0);
}

/*
 * dlpiExpecting - see if we got what we wanted.
 */
static int
dlpiExpecting(int prim, union DL_primitives *dlp)
{
	if (dlp->dl_primitive != (u_long) prim)
		return(-1);

	return(0);
}

/*
 * streamGetMsg - get a message from a stream, with timeout.
 */
static int
streamGetMsg(int fd, struct strbuf *ctlp, struct strbuf *datap, int *flagsp,
	     char *caller)
{
	int rc;

	/*
	 * Start timer.
	 */
	(void) sigset(SIGALRM, sigalrm);

	if (alarm(DLPI_MAXWAIT) < 0)
		fatalError("alarm", strerror(errno));

	/*
	 * Set flags argument and issue getmsg().
	 */
	*flagsp = 0;
	if ((rc = getmsg(fd, ctlp, datap, flagsp)) < 0)
		fatalError("streamGetMsg: getmsg", strerror(errno));

	/*
	 * Stop timer.
	 */
	if (alarm(0) < 0)
		fatalError("alarm", strerror(errno));

	/*
	 * Check for MOREDATA and/or MORECTL.
	 */
	if ((rc & (MORECTL | MOREDATA)) == (MORECTL | MOREDATA))
		return(-1);
	if (rc & MORECTL)
		return(-1);
	if (rc & MOREDATA)
		return(-1);

	/*
	 * Check for at least sizeof(long) control data portion.
	 */
	if (ctlp->len < sizeof(long))
		return(-1);

	return(0);
}

/*
 * sigalrm - handle alarms.
 */
static void
sigalrm(int sig)
{
	fprintf(stderr, "dlpi: timeout\n");
}
