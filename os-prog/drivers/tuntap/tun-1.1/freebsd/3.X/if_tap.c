/*
 * Copyright (c) 1999 Maksim Yevmenkin <m_evmenkin@yahoo.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * BASED ON:
 * -------------------------------------------------------------------------
 *
 *	$NetBSD: if_tun.c,v 1.14 1994/06/29 06:36:25 cgd Exp $
 *
 * Copyright (c) 1988, Julian Onions <jpo@cs.nott.ac.uk>
 * Nottingham University 1987.
 *
 * This source may be freely distributed, however I would be interested
 * in any changes that are made.
 *
 * This driver takes packets off the IP i/f and hands them up to a
 * user process to have its wicked way with. This driver has it's
 * roots in a similar driver written by Phil Cockcroft (formerly) at
 * UCL. This driver is based much more on read/write/poll mode of
 * operation though.
 */

/*
 * $Id: if_tap.c,v 1.3 2000/05/04 17:50:33 maxk Exp $
 */

#include <sys/param.h>
#include <sys/proc.h>
#include <sys/systm.h>
#include <sys/mbuf.h>
#include <sys/malloc.h>
#include <sys/socket.h>
#include <sys/filio.h>
#include <sys/sockio.h>
#include <sys/ttycom.h>
#include <sys/poll.h>
#include <sys/signalvar.h>
#include <sys/filedesc.h>
#include <sys/kernel.h>
#include <sys/sysctl.h>

#ifdef DEVFS
#include <sys/devfsext.h>
#endif /*DEVFS*/

#include <sys/conf.h>
#include <sys/uio.h>
#include <sys/vnode.h>

#include <net/ethernet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/netisr.h>
#include <net/route.h>

#ifdef INET
#include <netinet/in.h>
#include <netinet/in_var.h>
#endif /* INET */

#ifdef NS
#include <netns/ns.h>
#include <netns/ns_if.h>
#endif /* NS */

#if NBPFILTER > 0
#include <net/bpf.h>
#endif /* NBPFILTER */

#ifdef BRIDGE
#include <net/bridge.h>
#endif /* BRIDGE */

#include "if_tap.h"

/* pseudo device */
static void tapattach	__P((void *));
PSEUDO_SET(tapattach, if_tap);

/* network interface */
static void tapifstart	__P((struct ifnet *));
static int  tapifioctl	__P((struct ifnet *, u_long, caddr_t));
static void tapifinit	__P((void *));

/* character device */
static	d_open_t	tapopen;
static	d_close_t	tapclose;
static	d_read_t	tapread;
static	d_write_t	tapwrite;
static	d_ioctl_t	tapioctl;
static	d_poll_t	tappoll;

#define CDEV_MAJOR 200 /* 52 -- for tun */
static struct cdevsw tap_cdevsw = {
	tapopen,	tapclose,	tapread,	tapwrite,
	tapioctl,	nullstop,	noreset,	nodevtotty,
	tappoll,	nommap,		nostrategy,	"tap",	NULL,	-1
};

/* local storage */
static int			 tap_devsw_installed = 0;
static struct tap_softc		*tapcntl[NTAP] = { 0, };
static int			 tapdebug = 0;
#define TAPDEBUG		 if (tapdebug) printf
SYSCTL_INT(_debug, OID_AUTO, if_tap_debug, CTLFLAG_RW, &tapdebug, 0, "");

#define minor_val(n) ((((n) & ~0xff) << 8) | ((n) & 0xff))
#define dev_val(n) (((n) >> 8) | ((n) & 0xff))

/*
** tapattch 
**
** To attach pseudo device 
*/
static void
tapattach(dummy)
	void	*dummy;
{
	int		 	 i;
	struct ifnet		*ifp;
	struct tap_softc	*tp;
	dev_t			 dev;
	short			 macaddr_hi;

	TAPDEBUG(__FUNCTION__": attaching devices. NTAP = %d.\n", NTAP);

	if (tap_devsw_installed) {
		TAPDEBUG(__FUNCTION__": exiting. Devices already attached. " \
			"tap_devsw_installed = %d\n", tap_devsw_installed);
		return;
	}

	dev = makedev(CDEV_MAJOR, 0);
	cdevsw_add(&dev, &tap_cdevsw, NULL);
	tap_devsw_installed = 1;

	/* create network interfaces */
	for (i = 0; i < NTAP; i++) {
		tp = (struct tap_softc *)malloc(sizeof(*tp),M_DEVBUF,M_WAITOK);
		tapcntl[i] = tp;

		bzero(tp, sizeof(*tp));

		ifp = &tp->tap_if;
		tp->tap_flags = TAP_INITED;

		/* generate fake MAC address: 00 bd xx xx xx unit_no */
		macaddr_hi = htons(0x00bd);
		bcopy(&macaddr_hi, &tp->arpcom.ac_enaddr[0], sizeof(short));
		bcopy(&ticks, &tp->arpcom.ac_enaddr[2], sizeof(long));
		tp->arpcom.ac_enaddr[5] = (u_char)i;

		/* fill the rest and attach interface */	
		ifp->if_softc  = tp;
		ifp->if_unit   = i;
		ifp->if_name   = "tap";
		ifp->if_init   = tapifinit;
		ifp->if_output = ether_output;
		ifp->if_start  = tapifstart;
		ifp->if_ioctl  = tapifioctl;
		ifp->if_mtu    = ETHERMTU;
		ifp->if_flags  = IFF_BROADCAST | IFF_SIMPLEX | IFF_MULTICAST;

		ifp->if_snd.ifq_maxlen = ifqmaxlen;
		tp->if_rcv.ifq_maxlen  = ifqmaxlen;

		if_attach(ifp);
		ether_ifattach(ifp);

#if NBPFILTER > 0
		bpfattach(ifp, DLT_EN10MB, sizeof(struct ether_header));
#endif /* NBPFILTER */

#ifdef DEVFS
		tp->tap_devfs_token = devfs_add_devswf(&tap_cdevsw,minor_val(i),
						      DV_CHR, UID_UUCP,
						      GID_DIALER, 0600,
						      "tap%d", i);
#endif /* DEVFS */
	}
} /* tapattach */


/*
** tapopen 
**
** To open tunnel. Must be superuser & the device must be configured in
*/
static int
tapopen(dev, flag, mode, p)
	dev_t		 dev;
	int		 flag;
	int		 mode;
	struct proc	*p;
{
	struct ifnet		*ifp;
	struct tap_softc	*tp;
	register int		 unit, error;

	error = suser(p->p_ucred, &p->p_acflag);
	if (error) {
		return (error);
	}

	if ((unit = dev_val(minor(dev))) >= NTAP) {
		return (ENXIO);
	}

	tp = tapcntl[unit];
	ifp = &tp->tap_if;
	if (tp->tap_flags & TAP_OPEN) {
		return (EBUSY);
	}

	tp->tap_flags |= TAP_OPEN;

	TAPDEBUG(__FUNCTION__": %s%d device is open\n", 
						ifp->if_name, ifp->if_unit);
	return (0);
} /* tapopen */


/*
** tapclose
**
** Close the device - mark i/f down & delete routing info
*/
static int
tapclose(dev, foo, bar, p)
	dev_t		 dev;
	int		 foo;
	int		 bar;
	struct proc	*p;
{
	register int		 unit = dev_val(minor(dev)), s;
	struct tap_softc	*tp = tapcntl[unit];
	struct ifnet		*ifp = &tp->arpcom.ac_if;
	struct mbuf		*m;

	tp->tap_flags &= ~TAP_OPEN;

	/*
	 * junk all pending output
	 */

	do {
		s = splimp();
		IF_DEQUEUE(&ifp->if_snd, m);
		splx(s);

		if (m) {
			m_freem(m);
		}
	} while (m);

	do {
		s = splimp();
		IF_DEQUEUE(&tp->if_rcv, m);
		splx(s);

		if (m) {
			m_freem(m);
		}
	} while (m);

	if (ifp->if_flags & IFF_UP) {
		s = splimp();
		if_down(ifp);
		if (ifp->if_flags & IFF_RUNNING) {
			/* find internet addresses and delete routes */
			register struct ifaddr *ifa;

			for (ifa = ifp->if_addrhead.tqh_first; ifa;
						ifa = ifa->ifa_link.tqe_next) {
				if (ifa->ifa_addr->sa_family == AF_INET) {
					rtinit(ifa, (int)RTM_DELETE, 0);
#if 0
					rtinit(ifa, (int)RTM_DELETE,
						tp->tap_flags & TAP_DSTADDR ? 
								RTF_HOST : 0);
#endif

					/* remove address from interface */
					bzero(ifa->ifa_addr, 
						   sizeof(*(ifa->ifa_addr)));
					bzero(ifa->ifa_dstaddr, 
						   sizeof(*(ifa->ifa_dstaddr)));
					bzero(ifa->ifa_netmask, 
						   sizeof(*(ifa->ifa_netmask)));
				}
			}

			ifp->if_flags &= ~IFF_RUNNING;
		}
		splx(s);
	}

	funsetown(tp->tap_sigio);
	selwakeup(&tp->tap_rsel);

	TAPDEBUG(__FUNCTION__": %s%d device is closed\n", 
						ifp->if_name, ifp->if_unit);

	return (0);
} /* tapclose */


/*
** tapifinit
**
** Network interface initialization function
*/
static void
tapifinit(xtp)
	void	*xtp;
{
	struct tap_softc	*tp = (struct tap_softc *)xtp;
	struct ifnet		*ifp = &tp->tap_if;

	TAPDEBUG(__FUNCTION__": initializing %s%d...\n", 
						ifp->if_name, ifp->if_unit);

	ifp->if_flags |= IFF_RUNNING;
	ifp->if_flags &= ~IFF_OACTIVE;

	/* attempt to start output */
	tapifstart(ifp);
} /* tapifinit */


/*
** tapifioctl
**
** Process an ioctl request on network interface
*/
int
tapifioctl(ifp, cmd, data)
	struct ifnet	*ifp;
	u_long		 cmd;
	caddr_t		 data;
{
	int	s, error;

	s = splimp();

	switch (cmd) {
		case SIOCSIFADDR:
		case SIOCGIFADDR:
		case SIOCSIFMTU:
			error = ether_ioctl(ifp, cmd, data);
		break;

		case SIOCSIFFLAGS:
			if (ifp->if_flags & IFF_UP) {
				if ((ifp->if_flags & IFF_RUNNING) == 0) {
					tapifinit(ifp->if_softc);
				}
			}
			else {
				if (ifp->if_flags & IFF_RUNNING) {
					ifp->if_flags &= ~IFF_RUNNING;
				}
			}
			error = 0;
		break;

		case SIOCADDMULTI:
		case SIOCDELMULTI:
			error = 0;
		break;

		default:
			error = EINVAL;
		break;
	}
	splx(s);

	return (error);
} /* tapifioctl */


/*
** tapifstart 
** 
** Queue packets from higher level ready to put out.
*/
static void
tapifstart(ifp)
	struct ifnet	*ifp;
{
	struct tap_softc	*tp = tapcntl[ifp->if_unit];
	struct mbuf		*m0;
	int			 s;

	TAPDEBUG(__FUNCTION__": %s%d starting...\n", 
						ifp->if_name, ifp->if_unit);

	if ((tp->tap_flags & TAP_READY) != TAP_READY) {
		TAPDEBUG(__FUNCTION__": %s%d not ready. tap_flags = 0x%x\n", 
				ifp->if_name, ifp->if_unit, tp->tap_flags);
		return;
	}

	s = splimp();

	ifp->if_flags |= IFF_OACTIVE;
	for (;;) {
		IF_DEQUEUE(&ifp->if_snd, m0);
		if (!m0) {
			break;
		}

#if NBPFILTER > 0
		if (ifp->if_bpf) {
			bpf_mtap(ifp, m0);
		}
#endif /* NBPFILTER */

		if (IF_QFULL(&tp->if_rcv)) {
			IF_DROP(&tp->if_rcv); /* XXX: stat */

			TAPDEBUG(__FUNCTION__": %s%d if_rcv queue is full. " \
				"Dropping packet.\n", 
				ifp->if_name, ifp->if_unit);

			splx(s);
			m_freem(m0);

			return;
		}

		IF_ENQUEUE(&tp->if_rcv, m0);
	}

	if (tp->tap_flags & TAP_RWAIT) {
		tp->tap_flags &= ~TAP_RWAIT;
		wakeup((caddr_t)tp);
	}

	if (tp->tap_flags & TAP_ASYNC && tp->tap_sigio) {
		pgsigio(tp->tap_sigio, SIGIO, 0);
	}

	selwakeup(&tp->tap_rsel);

	ifp->if_flags &= ~IFF_OACTIVE;

	splx(s);
} /* tapifstart */


/*
** tapioctl
**
** the cdevsw interface is now pretty minimal.
*/
static int
tapioctl(dev, cmd, data, flag, p)
	dev_t		 dev;
	u_long		 cmd;
	caddr_t		 data;
	int		 flag;
	struct proc	*p;
{
	int			 unit = dev_val(minor(dev)), s;
	struct tap_softc	*tp = tapcntl[unit];
	struct ifnet		*ifp = &tp->tap_if;
 	struct tapinfo		*tapp;

	switch (cmd) {
 		case TAPSIFINFO:
 		        tapp = (struct tapinfo *)data;
 			ifp->if_mtu = tapp->mtu;
 			ifp->if_type = tapp->type;
 			ifp->if_baudrate = tapp->baudrate;
 		break;

	 	case TAPGIFINFO:
 			tapp = (struct tapinfo *)data;
 			tapp->mtu = ifp->if_mtu;
 			tapp->type = ifp->if_type;
 			tapp->baudrate = ifp->if_baudrate;
 		break;

		case TAPSDEBUG:
			tapdebug = *(int *)data;
		break;

		case TAPGDEBUG:
			*(int *)data = tapdebug;
		break;

		case FIONBIO:
		break;

		case FIOASYNC:
			if (*(int *)data) {
				tp->tap_flags |= TAP_ASYNC;
			}
			else {
				tp->tap_flags &= ~TAP_ASYNC;
			}
		break;

		case FIONREAD:
			s = splimp();
			if (tp->if_rcv.ifq_head) {
				struct mbuf *mb = tp->if_rcv.ifq_head;

				for(*(int *)data = 0; mb != 0; mb = mb->m_next) 
					*(int *)data += mb->m_len;
			} 
			else {
				*(int *)data = 0;
			}
			splx(s);
		break;

		case FIOSETOWN:
			return (fsetown(*(int *)data, &tp->tap_sigio));

		case FIOGETOWN:
			*(int *)data = fgetown(tp->tap_sigio);
			return (0);

		/* This is deprecated, FIOSETOWN should be used instead. */
		case TIOCSPGRP:
			return (fsetown(-(*(int *)data), &tp->tap_sigio));

		/* This is deprecated, FIOGETOWN should be used instead. */
		case TIOCGPGRP:
			*(int *)data = -fgetown(tp->tap_sigio);
			return (0);

		default:
			return (ENOTTY);
	}
	return (0);
} /* tapioctl */


/*
** tapread
**
** The cdevsw read interface - reads a packet at a time, or at
** least as much of a packet as can be read.
*/
static int
tapread(dev, uio, flag)
	dev_t dev;
	struct uio *uio;
	int flag;
{
	int			 unit = dev_val(minor(dev));
	struct tap_softc	*tp = tapcntl[unit];
	struct ifnet		*ifp = &tp->tap_if;
	struct mbuf		*m, *m0;
	int			 error = 0, len, s;

	TAPDEBUG(__FUNCTION__": %s%d reading...\n", ifp->if_name, ifp->if_unit);

	if ((tp->tap_flags & TAP_READY) != TAP_READY) {
		TAPDEBUG(__FUNCTION__": %s%d not ready. tap_flags = 0x%x\n", 
				ifp->if_name, ifp->if_unit, tp->tap_flags);
		return (EHOSTDOWN);
	}

	tp->tap_flags &= ~TAP_RWAIT;

	/* sleep until we get a packet */
	s = splimp();
	do {
		IF_DEQUEUE(&tp->if_rcv, m0);
		if (m0 == 0) {
			if (flag & IO_NDELAY) {
				splx(s);
				return (EWOULDBLOCK);
			}
			tp->tap_flags |= TAP_RWAIT;
			if (error = tsleep((caddr_t)tp, PCATCH | (PZERO + 1),
					"taprd", 0)) {
				splx(s);
				return (error);
			}
		}
	} while (m0 == 0);
	splx(s);

	/* xfer packet to user space */
	while (m0 && uio->uio_resid > 0 && error == 0) {
		len = min(uio->uio_resid, m0->m_len);
		if (len == 0) {
			break;
		}
		error = uiomove(mtod(m0, caddr_t), len, uio);
		MFREE(m0, m);
		m0 = m;
	}

	if (m0) {
		TAPDEBUG(__FUNCTION__": %s%d Dropping mbuf\n", 
						ifp->if_name, ifp->if_unit);
		m_freem(m0);
	}

	return (error);
} /* tapread */


/*
** tapwrite
**
** the cdevsw write interface - an atomic write is a packet - or else!
*/
static int
tapwrite(dev, uio, flag)
	dev_t		 dev;
	struct uio	*uio;
	int		 flag;
{
	int			 unit = dev_val(minor(dev));
	struct ifnet		*ifp = &tapcntl[unit]->tap_if;
	struct mbuf		*top, **mp, *m;
	int		 	 error = 0, tlen, mlen;
	struct ether_header	*eh;

	TAPDEBUG(__FUNCTION__": %s%d writting..\n", ifp->if_name, ifp->if_unit);

	if (uio->uio_resid == 0) {
		return (0); /* do nothing */
	}

	if (uio->uio_resid < 0 || uio->uio_resid > TAPMRU) {
		TAPDEBUG(__FUNCTION__": %s%d len = %d!\n", 
				ifp->if_name, ifp->if_unit, uio->uio_resid);
		return (EIO);
	}
	tlen = uio->uio_resid;

	/* get a header mbuf */
	MGETHDR(m, M_DONTWAIT, MT_DATA);
	if (m == NULL) {
		return (ENOBUFS);
	}
	mlen = MHLEN;

	top = 0;
	mp = &top;
	while (error == 0 && uio->uio_resid > 0) {
		m->m_len = min(mlen, uio->uio_resid);
		error = uiomove(mtod(m, caddr_t), m->m_len, uio);
		*mp = m;
		mp = &m->m_next;
		if (uio->uio_resid > 0) {
			MGET(m, M_DONTWAIT, MT_DATA);
			if (m == 0) {
				error = ENOBUFS;
				break;
			}
			mlen = MLEN;
		}
	}
	if (error) {
		if (top) {
			m_freem(top);
		}
		return (error);
	}

	top->m_pkthdr.len = tlen;
	top->m_pkthdr.rcvif = ifp;

	eh = mtod(top, struct ether_header *);

#if NBPFILTER > 0
	if (ifp->if_bpf) {
		bpf_mtap(ifp, top);
	}
#endif /* NBPFILTER */

#ifdef BRIDGE
	/* Ethernet bridge */
	if (do_bridge) {
		struct ifnet	*bdg_ifp;

		bdg_ifp = bridge_in(top);

		if (BDG_DROP == bdg_ifp) { /* drop it */
			m_freem(top);
			top = 0;
			return (0);
		}

		if (BDG_LOCAL != bdg_ifp) { /* forward it */
			bdg_forward(&top, bdg_ifp);
		}

		if (BDG_LOCAL == bdg_ifp ||
		    BDG_BCAST == bdg_ifp || BDG_MCAST == bdg_ifp) {
			goto getit;
		}

		/* drop it */
		if (top) {
			m_freem(top);
			top = 0;
		}
		return (0);
	}

getit:

#endif /* BRIDGE */

	/* adjust mbuf and give packet to the ether_input */
	m_adj(top, sizeof(struct ether_header));
	ether_input(ifp, eh, top);

	return (0);
} /* tapwrite */


/*
** tappoll
**
** The poll interface, this is only useful on reads
** really. The write detect always returns true, write never blocks
** anyway, it either accepts the packet or drops it.
*/
static int
tappoll(dev, events, p)
	dev_t		 dev;
	int		 events;
	struct proc	*p;
{
	int			 unit = dev_val(minor(dev)), s;
	struct tap_softc	*tp = tapcntl[unit];
	struct ifnet		*ifp = &tp->arpcom.ac_if;
	int		 	 revents = 0;

	s = splimp();
	TAPDEBUG(__FUNCTION__": %s%d polling...\n", ifp->if_name, ifp->if_unit);

	if (events & (POLLIN | POLLRDNORM)) {
		if (tp->if_rcv.ifq_len > 0) {
			TAPDEBUG(__FUNCTION__": %s%d have data! q = %d\n",
				ifp->if_name, ifp->if_unit, tp->if_rcv.ifq_len);
			revents |= (events & (POLLIN | POLLRDNORM));
		} 
		else {
			TAPDEBUG(__FUNCTION__": %s%d waiting for data\n",
				ifp->if_name, ifp->if_unit);
			selrecord(p, &tp->tap_rsel);
		}
	}

	if (events & (POLLOUT | POLLWRNORM)) {
		revents |= (events & (POLLOUT | POLLWRNORM));
	}

	splx(s);
	return (revents);
} /* tappoll */
