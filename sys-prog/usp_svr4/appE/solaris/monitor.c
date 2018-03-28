#ifndef lint
static char *RCSid = "$Header$";
#endif

/*
 * monitor.c - routines for monitoring the network
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
#include <sys/stream.h>
#include <sys/stropts.h>
#include <sys/bufmod.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_var.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp_var.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#include "defs.h"

extern u_long		networkMask;
extern u_long		networkAddress;
extern u_long		subNetworkMask;
extern u_long		subNetworkAddress;

static int		dlpiFileDesc = -1;
static IntervalCounters	*intervalCounter = NULL;

static void		processInterval(int);
static void		terminateMonitor(int);
static void		initializeCounters(void);
static void		processPacket(char *, struct sb_hdr *, u_int);

void
packetMonitor(void)
{
	int flags;
	u_int tdrops;
	extern int errno;
	struct timeval tv;
	struct sb_hdr *hdrp;
	struct itimerval itv;
	struct strbuf strdata;
	extern int intervalLength;
	char *bp, *buf, *pktp, *bufstop;

	/*
	 * Open the log file.
	 */
	openLogfile(O_WRONLY);

	/*
	 * Catch signals.
	 */
	sigset(SIGTERM, terminateMonitor);
	sigset(SIGQUIT, terminateMonitor);
	sigset(SIGINT,  terminateMonitor);

	/*
	 * Initialize the DLPI for capturing packets.
	 */
	if ((dlpiFileDesc = dlpiInitialize()) < 0)
		fatalError("DLPI initialization failed", NULL);

	/*
	 * Allocate a buffer to capture things into.
	 */
	if ((buf = malloc(DLPI_CHUNKSIZE)) == NULL)
		fatalError("not enough memory", NULL);

	/*
	 * Set up the interval timer.  We write a logfile record
	 * each time it goes off.
	 */
	memset((char *) &itv, 0, sizeof(struct itimerval));
	itv.it_interval.tv_sec = intervalLength;
	itv.it_value = itv.it_interval;

	if (setitimer(ITIMER_REAL, &itv, (struct itimerval *) 0) < 0)
		fatalError("setitimer", strerror(errno));
	
	sigset(SIGALRM, processInterval);

	/*
	 * Initialize the packet counters.
	 */
	initializeCounters();

	/*
	 * Flush anything queued up for us while we were doing
	 * other things.
	 */
	dlpiFlush(dlpiFileDesc);

	tdrops = 0;

	/*
	 * Forever...
	 */
	for (;;) {
		flags = 0;
		strdata.len = 0;
		strdata.buf = buf;
		strdata.maxlen = DLPI_CHUNKSIZE;

		/*
		 * Get a bunch of packets from the DLPI.
		 */
		if (getmsg(dlpiFileDesc, NULL, &strdata, &flags) != 0)
			continue;

		if (strdata.len <= 0)
			continue;

		bufstop = buf + strdata.len;

		/*
		 * Iterate through the buffer, extracting packets and
		 * processing them.
		 */
		for (bp = buf; bp < bufstop; bp += hdrp->sbh_totlen) {
			pktp = bp + sizeof(struct sb_hdr);
			hdrp = (struct sb_hdr *) bp;

			processPacket(pktp, hdrp, tdrops);

			tdrops = hdrp->sbh_drops;
		}
	}
}

/*
 * processPacket - figure out what a packet is and tabulate it.
 */
static void
processPacket(char *pktp, struct sb_hdr *hdrp, u_int tdrops)
{
	struct ip *ip;
	int hdrlen, bytes;
	struct tcphdr *tcp;
	struct udphdr *udp;
	u_long srcaddr, dstaddr;
	char packetData[ETHERMTU];
	struct ether_header *ether;

	/*
	 * Save the original length of the packet before it
	 * was truncated in the DLPI.
	 */
	bytes = hdrp->sbh_origlen;

	/*
	 * Basic counts.
	 */
	intervalCounter->i_totalPackets++;
	intervalCounter->i_totalBytes += bytes;
	intervalCounter->i_totalDropped += hdrp->sbh_drops - tdrops;

	/*
	 * First is the ethernet header.
	 */
	ether = (struct ether_header *) pktp;

	/*
	 * The 14-byte ethernet header screws up the alignment of
	 * everything else on machines like SPARC, so we have to
	 * copy the rest of the packet to realign it.  This is a
	 * small copy (54 bytes or less).
	 */
	memcpy(packetData, pktp + sizeof(struct ether_header),
	       hdrp->sbh_msglen - sizeof(struct ether_header));

	pktp = packetData;

	/*
	 * Find out what kind of ethernet packet it is.
	 */
	switch (ntohs(ether->ether_type)) {
	case ETHERPROTO_IP:
		intervalCounter->i_etherBytes[EPROTO_IP] += bytes;
		ip = (struct ip *) pktp;

		/*
		 * We do more with IP below.
		 */
		break;
	case ETHERPROTO_ARP:
		intervalCounter->i_etherBytes[EPROTO_ARP] += bytes;
		return;
	case ETHERPROTO_DECNET:
		intervalCounter->i_etherBytes[EPROTO_DECNET] += bytes;
		return;
	case ETHERPROTO_DECLAT:
		intervalCounter->i_etherBytes[EPROTO_DECLAT] += bytes;
		return;
	case ETHERPROTO_REVARP:
		intervalCounter->i_etherBytes[EPROTO_REVARP] += bytes;
		return;
	case ETHERPROTO_IPX:
		intervalCounter->i_etherBytes[EPROTO_IPX] += bytes;
		return;
	case ETHERPROTO_APPLETALK:
		intervalCounter->i_etherBytes[EPROTO_APPLETALK] += bytes;
		return;
	case ETHERPROTO_LOOPBACK:
		intervalCounter->i_etherBytes[EPROTO_LOOPBACK] += bytes;
		return;
	default:
		intervalCounter->i_etherBytes[EPROTO_UNKNOWN] += bytes;
		return;
	}

	/*
	 * Save the source and destination addresses.
	 */
	srcaddr = ntohl(ip->ip_src.s_addr);
	dstaddr = ntohl(ip->ip_dst.s_addr);

	/*
	 * If the destination is a multicast address, we classify things
	 * by the address, not by port numbers.
	 */
	if (IN_MULTICAST(dstaddr)) {
		/*
		 * General counter.
		 */
		intervalCounter->i_pathBytes[PKTPATH_MULTICAST] += bytes;

		/*
		 * Process address groups first by masking off bits.
		 * The masks should probably not be hard coded and we
		 * should make this cleaner, but there aren't enough
		 * of these to bother with that right now.
		 */
		switch (dstaddr & 0xffffff00) {
		case MULTICAST_RFEGENERIC:
			intervalCounter->i_mcastBytes[MCAST_RFEGENERIC] += bytes;
			return;
		case MULTICAST_RFEINDIVIDUAL:
			intervalCounter->i_mcastBytes[MCAST_RFEINDIVIDUAL] += bytes;
			return;
		}

		switch (dstaddr & 0xffff0000) {
		case MULTICAST_STGROUP:
			intervalCounter->i_mcastBytes[MCAST_STGROUP] += bytes;
			return;
		case MULTICAST_CONFERENCE:
			intervalCounter->i_mcastBytes[MCAST_CONFERENCE] += bytes;
			return;
		}

		switch (dstaddr & 0xff000000) {
		case MULTICAST_VMTPTRANSIENT:
			intervalCounter->i_mcastBytes[MCAST_VMTPTRANSIENT] += bytes;
			return;
		}

		/*
		 * If it's not one of the address groups, try looking at
		 * some of the fixed addresses.
		 */
		switch (dstaddr) {
		case MULTICAST_BASE:
			intervalCounter->i_mcastBytes[MCAST_BASE] += bytes;
			return;
		case MULTICAST_ALLSYSTEMS:
			intervalCounter->i_mcastBytes[MCAST_ALLSYSTEMS] += bytes;
			return;
		case MULTICAST_ALLROUTERS:
			intervalCounter->i_mcastBytes[MCAST_ALLROUTERS] += bytes;
			return;
		case MULTICAST_DVMRP:
			intervalCounter->i_mcastBytes[MCAST_DVMRP] += bytes;
			return;
		case MULTICAST_OSPFIGPALL:
			intervalCounter->i_mcastBytes[MCAST_OSPFIGPALL] += bytes;
			return;
		case MULTICAST_OSPFIGPDES:
			intervalCounter->i_mcastBytes[MCAST_OSPFIGPDES] += bytes;
			return;
		case MULTICAST_STROUTERS:
			intervalCounter->i_mcastBytes[MCAST_STROUTERS] += bytes;
			return;
		case MULTICAST_STHOSTS:
			intervalCounter->i_mcastBytes[MCAST_STHOSTS] += bytes;
			return;
		case MULTICAST_RIP2:
			intervalCounter->i_mcastBytes[MCAST_RIP2] += bytes;
			return;
		case MULTICAST_IGRP:
			intervalCounter->i_mcastBytes[MCAST_IGRP] += bytes;
			return;
		case MULTICAST_VMTP:
			intervalCounter->i_mcastBytes[MCAST_VMTP] += bytes;
			return;
		case MULTICAST_NTP:
			intervalCounter->i_mcastBytes[MCAST_NTP] += bytes;
			return;
		case MULTICAST_RWHO:
			intervalCounter->i_mcastBytes[MCAST_RWHO] += bytes;
			return;
		case MULTICAST_NISPLUS:
			intervalCounter->i_mcastBytes[MCAST_NISPLUS] += bytes;
			return;
		case MULTICAST_IETF1LAUDIO:
			intervalCounter->i_mcastBytes[MCAST_IETF1LAUDIO] += bytes;
			return;
		case MULTICAST_IETF1AUDIO:
			intervalCounter->i_mcastBytes[MCAST_IETF1AUDIO] += bytes;
			return;
		case MULTICAST_IETF1VIDEO:
			intervalCounter->i_mcastBytes[MCAST_IETF1VIDEO] += bytes;
			return;
		case MULTICAST_IETF2LAUDIO:
			intervalCounter->i_mcastBytes[MCAST_IETF2LAUDIO] += bytes;
			return;
		case MULTICAST_IETF2AUDIO:
			intervalCounter->i_mcastBytes[MCAST_IETF2AUDIO] += bytes;
			return;
		case MULTICAST_IETF2VIDEO:
			intervalCounter->i_mcastBytes[MCAST_IETF2VIDEO] += bytes;
			return;
		case MULTICAST_MUSIC:
			intervalCounter->i_mcastBytes[MCAST_MUSIC] += bytes;
			return;
		case MULTICAST_DVMRPMOSPF:
			intervalCounter->i_mcastBytes[MCAST_DVMRPMOSPF] += bytes;
			return;
		case MULTICAST_SUNRPC:
			intervalCounter->i_mcastBytes[MCAST_SUNRPC] += bytes;
			return;
		default:
			intervalCounter->i_mcastBytes[MCAST_UNKNOWN] += bytes;
			return;
		}
	}

	/*
	 * Okay, it's not a multicast packet.  Let's look at the source
	 * and destination and try to figure out why it's traversing this
	 * network.
	 */
	if (((srcaddr & subNetworkMask) == subNetworkAddress) &&
	    ((dstaddr & subNetworkMask) == subNetworkAddress))
		intervalCounter->i_pathBytes[PKTPATH_LOCALSUBNET] += bytes;
	else if (((srcaddr & networkMask) == networkAddress) &&
		 ((dstaddr & networkMask) == networkAddress))
		intervalCounter->i_pathBytes[PKTPATH_LOCALNET] += bytes;
	else
		intervalCounter->i_pathBytes[PKTPATH_FOREIGNNET] += bytes;

	/*
	 * Now look at IP protocols.
	 */
	switch (ntohs(ip->ip_p)) {
	case IPV4PROTO_ICMP:
		intervalCounter->i_ipBytes[IPROTO_ICMP] += bytes;
		return;
	case IPV4PROTO_IGMP:
		intervalCounter->i_ipBytes[IPROTO_IGMP] += bytes;
		return;
	case IPV4PROTO_GGP:
		intervalCounter->i_ipBytes[IPROTO_GGP] += bytes;
		return;
	case IPV4PROTO_IPIP:
		intervalCounter->i_ipBytes[IPROTO_IPIP] += bytes;
		return;
	case IPV4PROTO_TCP:
		intervalCounter->i_ipBytes[IPROTO_TCP] += bytes;
		hdrlen = ip->ip_hl * 4;

		tcp = (struct tcphdr *) (pktp + hdrlen);

		/*
		 * Count source and destination ports.
		 */
		intervalCounter->i_tcpSrcBytes[ntohs(tcp->th_sport)] += bytes;
		intervalCounter->i_tcpDstBytes[ntohs(tcp->th_dport)] += bytes;
		return;
	case IPV4PROTO_EGP:
		intervalCounter->i_ipBytes[IPROTO_EGP] += bytes;
		return;
	case IPV4PROTO_UDP:
		intervalCounter->i_ipBytes[IPROTO_UDP] += bytes;
		hdrlen = ip->ip_hl * 4;

		udp = (struct udphdr *) (pktp + hdrlen);

		/*
		 * Count source and destination ports.
		 */
		intervalCounter->i_udpSrcBytes[ntohs(udp->uh_sport)] += bytes;
		intervalCounter->i_udpDstBytes[ntohs(udp->uh_dport)] += bytes;
		return;
	case IPV4PROTO_ISOTP4:
		intervalCounter->i_ipBytes[IPROTO_ISOTP4] += bytes;
		return;
	case IPV4PROTO_HELLO:
		intervalCounter->i_ipBytes[IPROTO_HELLO] += bytes;
		return;
	case IPV4PROTO_SUNND:
		intervalCounter->i_ipBytes[IPROTO_SUNND] += bytes;
		return;
	case IPV4PROTO_ISOIP:
		intervalCounter->i_ipBytes[IPROTO_ISOIP] += bytes;
		return;
	case IPV4PROTO_IGRP:
		intervalCounter->i_ipBytes[IPROTO_IGRP] += bytes;
		return;
	case IPV4PROTO_OSPF:
		intervalCounter->i_ipBytes[IPROTO_OSPF] += bytes;
		return;
	default:
		intervalCounter->i_ipBytes[IPROTO_UNKNOWN] += bytes;
		return;
	}
}

/*
 * processInterval - do things when the interval timer expires.
 */
static void
processInterval(int signo)
{
	extern int errno;
	extern int maxIntervals;
	static int intervalNumber = 1;

	/*
	 * Record ending time.
	 */
	if (gettimeofday(&intervalCounter->i_stopTime, 0) < 0)
		fatalError("gettimeofday", strerror(errno));

	/*
	 * Write the current record.
	 */
	writeLogfileRecord(intervalCounter);

	/*
	 * Are we done?
	 */
	if (maxIntervals > 0) {
		if (intervalNumber++ == maxIntervals)
			terminateMonitor(0);
	}

	/*
	 * Initialize the counters again.
	 */
	initializeCounters();
}

/*
 * initializeCounters - set up an empty set of interval counters.
 */
static void
initializeCounters(void)
{
	extern int errno;

	/*
	 * Allocate a structure if needed.
	 */
	if (intervalCounter == NULL) {
		intervalCounter = (IntervalCounters *)
			malloc(sizeof(IntervalCounters));

		if (intervalCounter == NULL)
			fatalError("not enough memory", NULL);
	}

	/*
	 * Zero the structure.
	 */
	memset(intervalCounter, 0, sizeof(IntervalCounters));

	/*
	 * Record starting time.
	 */
	if (gettimeofday(&intervalCounter->i_startTime, 0) < 0)
		fatalError("gettimeofday", strerror(errno));

	/*
	 * Save network and subnetwork addresses.
	 */
	intervalCounter->i_network = networkAddress;
	intervalCounter->i_subNetwork = subNetworkAddress;
}

/*
 * terminateMonitor - write final logfile record, close files, and exit.
 */
static void
terminateMonitor(int signo)
{
	extern int errno;

	/*
	 * Write out the record we've been working on, if we were
	 * terminated by a signal.
	 */
	if (signo > 0) {
		/*
		 * Record ending time.
		 */
		if (gettimeofday(&intervalCounter->i_stopTime, 0) < 0)
			fatalError("gettimeofday", strerror(errno));

		writeLogfileRecord(intervalCounter);
	}

	/*
	 * Close the log file.
	 */
	closeLogfile();

	/*
	 * Close the DLPI interface.
	 */
	if (dlpiFileDesc > -1)
		close(dlpiFileDesc);

	exit(0);
}
