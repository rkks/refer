#ifndef lint
static char *RCSid = "$Header$";
#endif

/*
 * report.c - ruotines for generating a report
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
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>

#include "defs.h"

extern int	allPorts;
extern int	allIntervals;
extern int	maxIntervals;

static char	*tcpPorts[TCPPORT_MAX];		/* names of ports	*/
static char	*udpPorts[UDPPORT_MAX];		/* names of ports	*/

/*
 * Names of the packet types.
 */
static char	*etherTypes[] = {
	"Internet Protocol",
	"Address Resolution Protocol",
	"DECNET Phase IV",
	"DEC Local Area Transport (LAT)",
	"Reverse Address Resolution Proto",
	"Novel Netware (IPX)",
	"AppleTalk (EtherTalk)",
	"Ethernet Loopback",
	"Other"
};

/*
 * Names of the protocols.
 */
static char	*ipTypes[] = {
	"Internet Control Message Proto",
	"Internet Group Management Proto",
	"Gateway-to-Gateway Protocol",
	"IP-in-IP Encapsulation",
	"Transmission Control Protocol",
	"Exterior Gateway Protocol",
	"User Datagram Protocol",
	"ISO Transport Protocol Class 4",
	"HELLO Protocol",
	"Sun Network Disk Protocol",
	"ISO Internet Protocol",
	"Cisco Interior G/W Routing Proto",
	"Open Shortest Path First (OSPF)",
	"Other"
};

/*
 * Names of the protocols.
 */
static char	*mcastTypes[] = {
	"Multicast Base Address",
	"All Systems on This Subnet",
	"All Routers on This Subnet",
	"DVMRP Routers",
	"OSPFIGP All Routers",
	"OSPFIGP Designated Routers",
	"Stream Protocol Routers",
	"Stream Protocol Hosts",
	"Router Info Protocol v2 (RIP2)",
	"Interior Gateway Routing Proto",
	"Versatile Msg Transaction Proto",
	"Network Time Protocol",
	"Rwho",
	"Sun NIS+",
	"IETF-1 Low Audio",
	"IETF-1 Audio",
	"IETF-1 Video",
	"IETF-2 Low Audio",
	"IETF-2 Audio",
	"IETF-2 Video",
	"Music Service",
	"DVMRP on MOSPF",
	"Sun RPC PMAPPROC_CALLIT",
	"RFE Generic Service",
	"RFE Individual Conferences",
	"ST Multicast Groups",
	"Conference Calls (MBONE)",
	"VMTP Transient Groups",
	"Other"
};
	
static char	*fmt(double);
static void	getPortNames(void);
static void	printTotal(TotalCounters *);
static void	printInterval(IntervalCounters *);

/*
 * reportGenerator - read counter structures from the log file, total them
 *		     up, and print the results.
 */
void
reportGenerator(void)
{
	int n, nintervals;
	TotalCounters *total;
	IntervalCounters *interval;

	/*
	 * Load in the services file.
	 */
	getPortNames();

	/*
	 * Open the log file for reading.
	 */
	openLogfile(O_RDONLY);

	/*
	 * Allocate counters.
	 */
	interval = (IntervalCounters *) malloc(sizeof(IntervalCounters));
	total = (TotalCounters *) malloc(sizeof(TotalCounters));

	if (interval == NULL || total == NULL)
		fatalError("not enough memory", NULL);

	memset((char *) total, 0, sizeof(TotalCounters));

	nintervals = 0;

	/*
	 * For each counter structure...
	 */
	while (readLogfileRecord(interval) != 0) {
		/*
		 * If this is the first one, save the starting time.
		 */
		if (total->t_startTime.tv_sec == 0)
			total->t_startTime = interval->i_startTime;

		/*
		 * Save the stopping time.
		 */
		total->t_stopTime = interval->i_stopTime;

		/*
		 * Just copy this stuff.
		 */
		total->t_network = interval->i_network;
		total->t_subNetwork = interval->i_subNetwork;

		/*
		 * For all the rest, add the interval to the totals.
		 */
		total->t_totalBytes += interval->i_totalBytes;
		total->t_totalPackets += interval->i_totalPackets;
		total->t_totalDropped += interval->i_totalDropped;

		for (n = 0; n < IPROTO_MAX; n++)
			total->t_ipBytes[n] += interval->i_ipBytes[n];

		for (n = 0; n < EPROTO_MAX; n++)
			total->t_etherBytes[n] += interval->i_etherBytes[n];

		for (n = 0; n < PKTPATH_MAX; n++)
			total->t_pathBytes[n] += interval->i_pathBytes[n];

		for (n = 0; n < MCAST_MAX; n++)
			total->t_mcastBytes[n] += interval->i_mcastBytes[n];

		for (n = 0; n < TCPPORT_MAX; n++) {
			total->t_tcpSrcBytes[n] += interval->i_tcpSrcBytes[n];
			total->t_tcpDstBytes[n] += interval->i_tcpDstBytes[n];
		}

		for (n = 0; n < TCPPORT_MAX; n++) {
			total->t_udpSrcBytes[n] += interval->i_udpSrcBytes[n];
			total->t_udpDstBytes[n] += interval->i_udpDstBytes[n];
		}

		/*
		 * If the user asked for intermediate results,
		 * print the interval.
		 */
		if (allIntervals == TRUE)
			printInterval(interval);

		/*
		 * If the user set the maximum number of intervals,
		 * stop when we're done.
		 */
		if (++nintervals == maxIntervals)
			break;
	}

	/*
	 * Print the totals.
	 */
	printTotal(total);
	closeLogfile();
	exit(0);
}

/*
 * getPortNames - read the services file and save all the port names.
 */
static void
getPortNames(void)
{
	struct servent *sp;

	memset((char *) tcpPorts, 0, sizeof(tcpPorts));
	memset((char *) udpPorts, 0, sizeof(udpPorts));

	setservent(1);

	/*
	 * For each entry, save it.
	 */
	while ((sp = getservent()) != NULL) {
		switch (*(sp->s_proto)) {
		case 't':
			tcpPorts[ntohs(sp->s_port)] = strdup(sp->s_name);
			break;
		case 'u':
			udpPorts[ntohs(sp->s_port)] = strdup(sp->s_name);
			break;
		}
	}

	endservent();
}

/*
 * printInterval - print an interval counters structure.  We cheat and copy
 *		   it to a total counters structure, and then call printTotal.
 */
static void
printInterval(IntervalCounters *counter)
{
	int n;
	static TotalCounters *tmp = NULL;

	if (tmp == NULL) {
		tmp = (TotalCounters *) malloc(sizeof(TotalCounters));

		if (tmp == NULL)
			fatalError("not enough memory", NULL);
	}

	memset(tmp, 0, sizeof(TotalCounters));

	tmp->t_startTime = counter->i_startTime;

	tmp->t_stopTime = counter->i_stopTime;

	tmp->t_network = counter->i_network;
	tmp->t_subNetwork = counter->i_subNetwork;

	tmp->t_totalBytes = counter->i_totalBytes;
	tmp->t_totalPackets = counter->i_totalPackets;
	tmp->t_totalDropped = counter->i_totalDropped;

	for (n = 0; n < IPROTO_MAX; n++)
		tmp->t_ipBytes[n] = counter->i_ipBytes[n];

	for (n = 0; n < EPROTO_MAX; n++)
		tmp->t_etherBytes[n] = counter->i_etherBytes[n];

	for (n = 0; n < PKTPATH_MAX; n++)
		tmp->t_pathBytes[n] = counter->i_pathBytes[n];

	for (n = 0; n < MCAST_MAX; n++)
		tmp->t_mcastBytes[n] = counter->i_mcastBytes[n];

	for (n = 0; n < TCPPORT_MAX; n++) {
		tmp->t_tcpSrcBytes[n] = counter->i_tcpSrcBytes[n];
		tmp->t_tcpDstBytes[n] = counter->i_tcpDstBytes[n];
	}

	for (n = 0; n < TCPPORT_MAX; n++) {
		tmp->t_udpSrcBytes[n] = counter->i_udpSrcBytes[n];
		tmp->t_udpDstBytes[n] = counter->i_udpDstBytes[n];
	}

	printTotal(tmp);
	putchar('\f');
}

/*
 * printTotal - print out a total counters structure.
 */
static void
printTotal(TotalCounters *counter)
{
	int n;
	u_long addr;
	char tmp[64];
	double seconds;

	/*
	 * Semantics.  If they're not subnetting, then these are
	 * local network bytes, not local subnetwork bytes.
	 */
	if (counter->t_network == counter->t_subNetwork) {
		counter->t_pathBytes[PKTPATH_LOCALNET] =
			counter->t_pathBytes[PKTPATH_LOCALSUBNET];
		counter->t_pathBytes[PKTPATH_LOCALSUBNET] = 0.0;
	}

	/*
	 * Print a header.
	 */
	addr = counter->t_subNetwork;
	sprintf(tmp, "%d.%d.%d.%d", (addr >> 24) & 0xff, (addr >> 16) & 0xff,
		(addr >> 8) & 0xff, addr & 0xff);
	seconds = counter->t_stopTime.tv_sec - counter->t_startTime.tv_sec;

	printf("%80s", "PACKET MONITORING STATISTICS FOR\n");
	printf("%*s\n", 64 + (int) strlen(tmp) / 2, tmp);
	printf("%62.24s - ",
	       ctime(&counter->t_startTime.tv_sec));
	printf("%.24s\n",
	       ctime(&counter->t_stopTime.tv_sec));
	putchar('\n');


	/*
	 * Print basic totals and some averages derived from them.
	 */
	printf("Total Bytes Captured:            %20s             ",
	       fmt(counter->t_totalBytes));
	printf("Average Bytes/Packet:            %20s\n",
	       fmt(counter->t_totalBytes / counter->t_totalPackets));
	printf("Total Packets Captured:          %20s             ",
	       fmt(counter->t_totalPackets));
	printf("Average Bytes/Second:            %20s\n",
	       fmt(counter->t_totalBytes / seconds));
	printf("Total Packets Dropped:           %20s             ",
	       fmt(counter->t_totalDropped));
	printf("Average Packets/Second:          %20s\n",
	       fmt(counter->t_totalPackets / seconds));
	putchar('\n');

	/*
	 * Print packet path statistics.
	 */
	printf("%86s\n%87s\n\n", "INTERNET PROTOCOL PACKET ROUTING STATISTICS",
	       "(total in bytes, percentage of total ip bytes)");
	printf("Local Subnetwork Traffic:        %20s (%5.1f%%)    ",
	       fmt(counter->t_pathBytes[PKTPATH_LOCALSUBNET]),
	       counter->t_pathBytes[PKTPATH_LOCALSUBNET] /
	       counter->t_etherBytes[EPROTO_IP] * 100.0);
	printf("Foreign Network Traffic:         %20s (%5.1f%%)\n",
	       fmt(counter->t_pathBytes[PKTPATH_FOREIGNNET]),
	       counter->t_pathBytes[PKTPATH_FOREIGNNET] /
	       counter->t_etherBytes[EPROTO_IP] * 100.0);
	printf("Local Network Traffic:           %20s (%5.1f%%)    ",
	       fmt(counter->t_pathBytes[PKTPATH_LOCALNET]),
	       counter->t_pathBytes[PKTPATH_LOCALNET] /
	       counter->t_etherBytes[EPROTO_IP] * 100.0);
	printf("Multicast Traffic:               %20s (%5.1f%%)\n",
	       fmt(counter->t_pathBytes[PKTPATH_MULTICAST]),
	       counter->t_pathBytes[PKTPATH_MULTICAST] /
	       counter->t_etherBytes[EPROTO_IP] * 100.0);
	putchar('\n');

	/*
	 * Print raw packet type statistics.
	 */
	printf("%80s\n%86s\n\n", "ETHERNET PACKET TYPE STATISTICS",
	       "(total in bytes, percentage of total bytes)");

	for (n = 0; n < EPROTO_MAX; n++) {
		printf("%-32s %20s (%5.1f%%)", etherTypes[n],
		       fmt(counter->t_etherBytes[n]),
		       counter->t_etherBytes[n] /
		       counter->t_totalBytes * 100.0);

		if (n & 1)
			putchar('\n');
		else
			printf("    ");
	}

	if (n & 1)
		putchar('\n');

	/*
	 * Print IP packet type statistics.
	 */
	printf("\n%78s\n%87s\n\n", "INTERNET PROTOCOL STATISTICS",
	       "(total in bytes, percentage of total ip bytes)");

	for (n = 0; n < IPROTO_MAX; n++) {
		printf("%-32s %20s (%5.1f%%)", ipTypes[n],
		       fmt(counter->t_ipBytes[n]),
		       counter->t_ipBytes[n] /
		       counter->t_etherBytes[EPROTO_IP] * 100.0);

		if (n & 1)
			putchar('\n');
		else
			printf("    ");
	}

	/*
	 * Print this again here, because IP Multicast is part of
	 * the IP totals, but is not counted in the ipBytes array
	 * (see monitor.c).
	 */
	printf("%-32s %20s (%5.1f%%)\n", "IP Multicast",
	       fmt(counter->t_pathBytes[PKTPATH_MULTICAST]),
	       counter->t_pathBytes[PKTPATH_MULTICAST] /
	       counter->t_etherBytes[EPROTO_IP] * 100.0);

	/*
	 * Print IP multicast statistics.
	 */
	printf("\n%76s\n%89s\n\n", "IP MULTICAST STATISTICS",
	       "(total in bytes, percentage of ip multicast bytes)");

	for (n = 0; n < MCAST_MAX; n++) {
		printf("%-32s %20s (%5.1f%%)", mcastTypes[n],
		       fmt(counter->t_mcastBytes[n]),
		       counter->t_mcastBytes[n] /
		       counter->t_pathBytes[PKTPATH_MULTICAST] * 100.0);

		if (n & 1)
			putchar('\n');
		else
			printf("    ");
	}

	if (n & 1)
		putchar('\n');

	/*
	 * Print TCP statistics.
	 */
	printf("\f%71s\n%85s\n\n", "TCP STATISTICS",
	       "(total in bytes, percentage of tcp bytes)");
	printf("%20s%33s%33s%33s\n", "Service", "Source", "Destination",
	       "Total");

	for (n = 0; n < TCPPORT_MAX; n++) {
		/*
		 * Skip empty counters.
		 */
		if (counter->t_tcpSrcBytes[n] == 0 &&
		    counter->t_tcpDstBytes[n] == 0)
			continue;

		/*
		 * Only print named services, unless they asked
		 * for everything.
		 */
		if (tcpPorts[n] != NULL) {
			printf("%-32s ", tcpPorts[n]);
			printf("%20s (%5.1f%%)    ",
			       fmt(counter->t_tcpSrcBytes[n]),
			       counter->t_tcpSrcBytes[n] /
			       counter->t_ipBytes[IPROTO_TCP] * 100.0);
			printf("%20s (%5.1f%%)    ",
			       fmt(counter->t_tcpDstBytes[n]),
			       counter->t_tcpDstBytes[n] /
			       counter->t_ipBytes[IPROTO_TCP] * 100.0);
			printf("%20s (%5.1f%%)\n",
			       fmt(counter->t_tcpSrcBytes[n] +
				   counter->t_tcpDstBytes[n]),
			       (counter->t_tcpSrcBytes[n] +
				counter->t_tcpDstBytes[n]) /
			       counter->t_ipBytes[IPROTO_TCP] * 100.0);
		}
		else {
			if (allPorts == TRUE) {
				printf("%-32d ", n);
				printf("%20s (%5.1f%%)    ",
				       fmt(counter->t_tcpSrcBytes[n]),
				       counter->t_tcpSrcBytes[n] /
				       counter->t_ipBytes[IPROTO_TCP] * 100.0);
				printf("%20s (%5.1f%%)    ",
				       fmt(counter->t_tcpDstBytes[n]),
				       counter->t_tcpDstBytes[n] /
				       counter->t_ipBytes[IPROTO_TCP] * 100.0);
				printf("%20s (%5.1f%%)\n",
				       fmt(counter->t_tcpSrcBytes[n] +
					   counter->t_tcpDstBytes[n]),
				       (counter->t_tcpSrcBytes[n] +
					counter->t_tcpDstBytes[n]) /
				       counter->t_ipBytes[IPROTO_TCP] * 100.0);
			}
		}
	}

	/*
	 * Print UDP statistics.
	 */
	printf("\f%71s\n%85s\n\n", "UDP STATISTICS",
	       "(total in bytes, percentage of udp bytes)");
	printf("%20s%33s%33s%33s\n", "Service", "Source", "Destination",
	       "Total");

	for (n = 0; n < UDPPORT_MAX; n++) {
		/*
		 * Skip empty counters.
		 */
		if (counter->t_udpSrcBytes[n] == 0 &&
		    counter->t_udpDstBytes[n] == 0)
			continue;

		/*
		 * Only print named ports, unless they
		 * asked for everything.
		 */
		if (udpPorts[n] != NULL) {
			printf("%-32s ", udpPorts[n]);
			printf("%20s (%5.1f%%)    ",
			       fmt(counter->t_udpSrcBytes[n]),
			       counter->t_udpSrcBytes[n] /
			       counter->t_ipBytes[IPROTO_UDP] * 100.0);
			printf("%20s (%5.1f%%)    ",
			       fmt(counter->t_udpDstBytes[n]),
			       counter->t_udpDstBytes[n] /
			       counter->t_ipBytes[IPROTO_UDP] * 100.0);
			printf("%20s (%5.1f%%)\n",
			       fmt(counter->t_udpSrcBytes[n] +
				   counter->t_udpDstBytes[n]),
			       (counter->t_udpSrcBytes[n] +
				counter->t_udpDstBytes[n]) /
			       counter->t_ipBytes[IPROTO_UDP] * 100.0);
		}
		else {
			if (allPorts == TRUE) {
				printf("%-32d ", n);
				printf("%20s (%5.1f%%)    ",
				       fmt(counter->t_udpSrcBytes[n]),
				       counter->t_udpSrcBytes[n] /
				       counter->t_ipBytes[IPROTO_UDP] * 100.0);
				printf("%20s (%5.1f%%)    ",
				       fmt(counter->t_udpDstBytes[n]),
				       counter->t_udpDstBytes[n] /
				       counter->t_ipBytes[IPROTO_UDP] * 100.0);
				printf("%20s (%5.1f%%)\n",
				       fmt(counter->t_udpSrcBytes[n] +
					   counter->t_udpDstBytes[n]),
				       (counter->t_udpSrcBytes[n] +
					counter->t_udpDstBytes[n]) /
				       counter->t_ipBytes[IPROTO_UDP] * 100.0);
			}
		}
	}
}

/*
 * fmt - format a number with commas every three digits, to make large
 *	 numbers more easily readable.
 */
static char *
fmt(double f)
{
	int len;
	char *p, *q;
	char tmp[BUFSIZ];
	static char buf[BUFSIZ];

	/*
	 * Convert to a string.
	 */
	sprintf(tmp, "%.0f", f);
	len = strlen(tmp);

	/*
	 * No commas.
	 */
	if (len < 4) {
		strcpy(buf, tmp);
		return(buf);
	}

	/*
	 * Add commas in the right places.
	 */
	for (p = buf, q = tmp; len > 0; len--) {
		if (((len % 3) == 0) && (p != buf))
			*p++ = ',';
		
		*p++ = *q++;
	}

	*p++ = '\0';
	return(buf);
}
