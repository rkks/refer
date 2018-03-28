/*
 * $Header$
 *
 * defs.h - definitions for packmon
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

/*
 * Default values you can change.
 */
#define DEFAULT_INTERVAL	3600
#define DEFAULT_LOGFILE		"packmon.log"
#define DEFAULT_PORTLIST	"/etc/services"

/*
 * True and false.
 */
#ifdef TRUE
#undef TRUE
#endif

#ifndef FALSE
#undef FALSE
#endif

#define TRUE			1
#define FALSE			0

/*
 * Total number of TCP and UDP port numbers.
 */
#define TCPPORT_MAX		65536	/* unsigned short		*/
#define UDPPORT_MAX		65536	/* unsigned short		*/

/*
 * Ethernet type field values (selected).  See the Assigned Numbers RFC.
 */
#define ETHERPROTO_IP		0x0800	/* Internet Protocol		*/
#define ETHERPROTO_ARP		0x0806	/* Address Resolution Protocol	*/
#define ETHERPROTO_DECNET	0x6003	/* DECNET Phase IV		*/
#define ETHERPROTO_DECLAT	0x6004	/* DEC Local Area Transport LAT	*/
#define ETHERPROTO_REVARP	0x8035	/* Reverse Address Resolution	*/
#define ETHERPROTO_IPX		0x8037	/* Novell Netware		*/
#define ETHERPROTO_APPLETALK	0x809b	/* Apple EtherTalk		*/
#define ETHERPROTO_LOOPBACK	0x9000	/* Loopback			*/

/*
 * Same as above, but for subscripting into the i_etherBytes and
 * t_etherBytes arrays.
 */
#define EPROTO_IP		0
#define EPROTO_ARP		1
#define EPROTO_DECNET		2
#define EPROTO_DECLAT		3
#define EPROTO_REVARP		4
#define EPROTO_IPX		5
#define EPROTO_APPLETALK	6
#define EPROTO_LOOPBACK		7
#define EPROTO_UNKNOWN		8

#define EPROTO_MAX		9

/*
 * IP Version 4 protocol numbers (selected).  See the Assigned Numbers RFC.
 */
#define IPV4PROTO_ICMP		1	/* Internet Control Message	*/
#define IPV4PROTO_IGMP		2	/* Internet Group Management	*/
#define IPV4PROTO_GGP		3	/* Gateway-to-Gateway Protocol	*/
#define IPV4PROTO_IPIP		4	/* IP-in-IP Encapsulation	*/
#define IPV4PROTO_TCP		6	/* Transmission Control Protocol*/
#define IPV4PROTO_EGP		8	/* Exterior Gateway Protocol	*/
#define IPV4PROTO_UDP		17	/* User Datagram Protocol	*/
#define IPV4PROTO_ISOTP4	29	/* ISO Transport Proto Class 4	*/
#define IPV4PROTO_HELLO		63	/* HELLO Protocol		*/
#define IPV4PROTO_SUNND		77	/* Sun Network Disk Protocol	*/
#define IPV4PROTO_ISOIP		80	/* ISO Internet Protocol	*/
#define IPV4PROTO_IGRP		88	/* Cisco Interior G/W Routing	*/
#define IPV4PROTO_OSPF		89	/* Open Shortest Path First	*/

/*
 * Same as above, but for subscripting into the i_ipBytes and
 * t_ipBytes arrays.
 */
#define IPROTO_ICMP		0
#define IPROTO_IGMP		1
#define IPROTO_GGP		2
#define IPROTO_IPIP		3
#define IPROTO_TCP		4
#define IPROTO_EGP		5
#define IPROTO_UDP		6
#define IPROTO_ISOTP4		7
#define IPROTO_HELLO		8
#define IPROTO_SUNND		9
#define IPROTO_ISOIP		10
#define IPROTO_IGRP		11
#define IPROTO_OSPF		12
#define IPROTO_UNKNOWN		13

#define IPROTO_MAX		14

/*
 * Multicast addresses (selected).  See the Assigned Numbers RFC.
 */
#define MULTICAST_BASE		0xe0000000 /* Base Address		*/
#define MULTICAST_ALLSYSTEMS	0xe0000001 /* All systems on subnet	*/
#define MULTICAST_ALLROUTERS	0xe0000002 /* All routers on subnet	*/
#define MULTICAST_DVMRP		0xe0000004 /* DVMRP routers		*/
#define MULTICAST_OSPFIGPALL	0xe0000005 /* OSPFIGP all routers	*/
#define MULTICAST_OSPFIGPDES	0xe0000006 /* OSPFIGP designated routers*/
#define MULTICAST_STROUTERS	0xe0000007 /* Stream Protocol routers	*/
#define MULTICAST_STHOSTS	0xe0000008 /* Stream Protocol hosts	*/
#define MULTICAST_RIP2		0xe0000009 /* Router Info Protocol v2	*/
#define MULTICAST_IGRP		0xe000000a /* Interior Gateway Routing	*/
#define MULTICAST_VMTP		0xe0000100 /* Versatile Msg Transact.	*/
#define MULTICAST_NTP		0xe0000101 /* Network Time Protocol	*/
#define MULTICAST_RWHO		0xe0000103 /* Rwho			*/
#define MULTICAST_NISPLUS	0xe0000108 /* Sun NIS+			*/
#define MULTICAST_IETF1LAUDIO	0xe000010a /* IETF 1 Low Audio		*/
#define MULTICAST_IETF1AUDIO	0xe000010b /* IETF 1 Audio		*/
#define MULTICAST_IETF1VIDEO	0xe000010c /* IETF 1 Video		*/
#define MULTICAST_IETF2LAUDIO	0xe000010d /* IETF 2 Low Audio		*/
#define MULTICAST_IETF2AUDIO	0xe000010e /* IETF 2 Audio		*/
#define MULTICAST_IETF2VIDEO	0xe000010f /* IETF 2 Video		*/
#define MULTICAST_MUSIC		0xe0000110 /* Music Service		*/
#define MULTICAST_DVMRPMOSPF	0xe0000115 /* DVMRP on MOSPF		*/
#define MULTICAST_SUNRPC	0xe0000202 /* Sun RPC PMAPPROC_CALLIT	*/

/*
 * Multicast address groups (selected).  See the Assigned Numbers RFC.
 */
#define MULTICAST_RFEGENERIC	0xe0000300 /* RFE Generic Service	*/
#define MULTICAST_RFEINDIVIDUAL	0xe0000400 /* RFE Individual Conferences*/
#define MULTICAST_STGROUP	0xe0010000 /* ST multicast groups	*/
#define MULTICAST_CONFERENCE	0xe0020000 /* Conference Calls (MBONE)	*/
#define MULTICAST_VMTPTRANSIENT	0xe8000000 /* VMTP transient groups	*/

/*
 * Same as above, but for subscripting into the i_mcastBytes and
 * t_mcastBytes arrays.
 */
#define MCAST_BASE		0
#define MCAST_ALLSYSTEMS	1
#define MCAST_ALLROUTERS	2
#define MCAST_DVMRP		3
#define MCAST_OSPFIGPALL	4
#define MCAST_OSPFIGPDES	5
#define MCAST_STROUTERS		6
#define MCAST_STHOSTS		7
#define MCAST_RIP2		8
#define MCAST_IGRP		9
#define MCAST_VMTP		10
#define MCAST_NTP		11
#define MCAST_RWHO		12
#define MCAST_NISPLUS		13
#define MCAST_IETF1LAUDIO	14
#define MCAST_IETF1AUDIO	15
#define MCAST_IETF1VIDEO	16
#define MCAST_IETF2LAUDIO	17
#define MCAST_IETF2AUDIO	18
#define MCAST_IETF2VIDEO	19
#define MCAST_MUSIC		20
#define MCAST_DVMRPMOSPF	21
#define MCAST_SUNRPC		22
#define MCAST_RFEGENERIC	23
#define MCAST_RFEINDIVIDUAL	24
#define MCAST_STGROUP		25
#define MCAST_CONFERENCE	26
#define MCAST_VMTPTRANSIENT	27
#define MCAST_UNKNOWN		28

#define MCAST_MAX		29

/*
 * Packet path classifications.
 */
#define PKTPATH_LOCALNET	0	/* Local (Class X) network	*/
#define PKTPATH_LOCALSUBNET	1	/* Local subnetwork		*/
#define PKTPATH_FOREIGNNET	2	/* Foreign network		*/
#define PKTPATH_MULTICAST	3	/* Multicast			*/

#define PKTPATH_MAX		4

/*
 * Constants for the DLPI interface.
 */
#define DLPI_DEVDIR		"/dev/"		/* path to device dir	*/
#define DLPI_BUFMOD		"bufmod"	/* buffering module	*/
#define DLPI_MAXWAIT		15		/* stream ops timeout	*/
#define DLPI_TRUNCATE		54		/* packet truncation	*/
#define DLPI_MAXDLBUF		8192		/* max dlpi buf size	*/
#define DLPI_CHUNKSIZE		(8 * 8192)	/* buffering unit	*/

/*
 * Complete set of counters used for each interval.
 */
typedef struct {
	struct timeval		i_startTime;
	struct timeval		i_stopTime;

	u_long			i_network;
	u_long			i_subNetwork;

	u_long			i_totalBytes;
	u_long			i_totalPackets;
	u_long			i_totalDropped;

	u_long			i_ipBytes[IPROTO_MAX];
	u_long			i_etherBytes[EPROTO_MAX];
	u_long			i_pathBytes[PKTPATH_MAX];

	u_long			i_mcastBytes[MCAST_MAX];

	u_long			i_tcpSrcBytes[TCPPORT_MAX];
	u_long			i_tcpDstBytes[TCPPORT_MAX];
	u_long			i_udpSrcBytes[UDPPORT_MAX];
	u_long			i_udpDstBytes[UDPPORT_MAX];
} IntervalCounters;

/*
 * Complete set of counters used for totalling up intervals.  This is
 * the same as IntervalCounters, but with bigger data types.
 */
typedef struct {
	struct timeval		t_startTime;
	struct timeval		t_stopTime;

	double			t_network;
	double			t_subNetwork;

	double			t_totalBytes;
	double			t_totalPackets;
	double			t_totalDropped;

	double			t_ipBytes[IPROTO_MAX];
	double			t_etherBytes[EPROTO_MAX];
	double			t_pathBytes[PKTPATH_MAX];

	double			t_mcastBytes[MCAST_MAX];

	double			t_tcpSrcBytes[TCPPORT_MAX];
	double			t_tcpDstBytes[TCPPORT_MAX];
	double			t_udpSrcBytes[UDPPORT_MAX];
	double			t_udpDstBytes[UDPPORT_MAX];
} TotalCounters;

/*
 * Function prototypes.
 */
void		usage(void);
void		dlpiFlush(int);
void		openLogfile(int);
int		main(int, char **);
void		closeLogfile(void);
void		packetMonitor(void);
int		dlpiInitialize(void);
void		reportGenerator(void);
void		fatalError(char *, char *);
int		readLogfileRecord(IntervalCounters *);
void		writeLogfileRecord(IntervalCounters *);
