/* Autori: Giuseppe Fioretti e Andrea De Vendictis*/

#include <stdio.h>
#include <math.h>
#include <time.h>


#define WARNING 1
#define FATAL 2
#define INTERVALLO 10000             /*inter-departure time*/
#define MAX_PAC  1000000              /*max packets number*/
#define N_PACCHETTI 1000000           /*default transmitted packets  */

/*struct Stat_Part(float media, float deviazione);*/
void Inizializza();
void errore ( short livello, char *str, short linea );
void modifica_scala_tempi(void);
void Riempie_Ritardi ( long indice, long ritardo );
void Stampa();
void Riempie_Partenze();
/*struct Stat_Part Statistiche_Partenze();*/


struct timeval partenze[MAX_PAC];     /*departure time*/
long ritardi[MAX_PAC];                /*round trip delays        */
char *nomefile;                       /*output file    */
long fuoriseq=0;                      /*out of order packets*/
long prec=0;                          
                                      
// #####   P I N G      S O U R C E  ######################

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Muuss.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

char copyright[] =
  "@(#) Copyright (c) 1989 The Regents of the University of California.\n"
  "All rights reserved.\n";
/*
 * From: @(#)ping.c	5.9 (Berkeley) 5/12/91
 */
char rcsid[] = "$Id: ping.c,v 1.22 1997/06/08 19:39:47 dholland Exp $";
char pkg[] = "netkit-base-0.10";

/*
 *			P I N G . C
 *
 * Using the InterNet Control Message Protocol (ICMP) "ECHO" facility,
 * measure round-trip-delays and packet loss across network paths.
 *
 * Author -
 *	Mike Muuss
 *	U. S. Army Ballistic Research Laboratory
 *	December, 1983
 *
 * Status -
 *	Public Domain.  Distribution Unlimited.
 * Bugs -
 *	More statistics could always be gathered.
 *	This program has to run SUID to ROOT to access the ICMP socket.
 */

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/signal.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/*
 * Note: on some systems dropping root makes the process dumpable or
 * traceable. In that case if you enable dropping root and someone
 * traces ping, they get control of a raw socket and can start
 * spoofing whatever packets they like. SO BE CAREFUL.
 */
#ifdef __linux__
#define SAFE_TO_DROP_ROOT
#endif

#if defined(__GLIBC__) && (__GLIBC__ >= 2)
#define icmphdr			icmp


#else
#define ICMP_MINLEN	28
#define inet_ntoa(x) inet_ntoa(*((struct in_addr *)&(x)))
#endif


#define	DEFDATALEN	(12)	/* minima lunghezza necessaria per il timeval e il contatore di sequenza*/

#define	MAXIPLEN	60
#define	MAXICMPLEN	76
#define	MAXPACKET	(65536 - 60 - 8)/* max packet size */
#define	MAXWAIT		300		/* max seconds to wait for response MODIFICA*/
#define	NROUTES		9		/* number of record route slots */

#define	A(bit)		rcvd_tbl[(bit)>>3]	/* identify byte in array */
#define	B(bit)		(1 << ((bit) & 0x07))	/* identify bit in byte */
#define	SET(bit)	(A(bit) |= B(bit))
#define	CLR(bit)	(A(bit) &= (~B(bit)))
#define	TST(bit)	(A(bit) & B(bit))

/* various options */
int options;
#define	F_FLOOD		0x001
#define	F_INTERVAL	0x002
#define	F_NUMERIC	0x004
#define	F_PINGFILLED	0x008
#define	F_QUIET		0x010
#define	F_RROUTE	0x020
#define	F_SO_DEBUG	0x040
#define	F_SO_DONTROUTE	0x080
#define	F_VERBOSE	0x100


/*
 * MAX_DUP_CHK is the number of bits in received table, i.e. the maximum
 * number of received sequence numbers we can keep track of.  Change 128
 * to 8192 for complete accuracy...
 */
#define	MAX_DUP_CHK	(8 * 128)
int mx_dup_ck = MAX_DUP_CHK;
char rcvd_tbl[MAX_DUP_CHK / 8];

struct sockaddr whereto;	/* who to ping */
int datalen = DEFDATALEN;
int s;				/* socket file descriptor */
u_char outpack[MAXPACKET];
char BSPACE = '\b';		/* characters written for flood */
char DOT = '.';
static char *hostname;
static int ident;		/* process id to identify our packets */

/* counters */
static long npackets=N_PACCHETTI;		/* max packets to transmit */
static long nreceived=0;		/* # of packets we got back */
static long nrepeats=0;		/* number of duplicates */
static long ntransmitted=0;	/* sequence # for outbound packets = #sent */
static int interval = 1;	/* interval between packets */

/* timing */
static int timing;		/* flag to do timing */
static long tmin = LONG_MAX;	/* minimum round trip time */
static long tmax = 0;		/* maximum round trip time */
static u_long tsum;		/* sum of all times, for doing average */

/* protos */
static char *pr_addr(u_long);
static int in_cksum(u_short *addr, int len);
static void catcher(int);
static void finish(int ignore);
static void pinger();
static void usage(void);
static void pr_pack(char *buf, int cc, struct sockaddr_in *from);
static void tvsub(struct timeval *out, struct timeval *in);



int
main(int argc, char *argv[])
{
	/*struct timeval timeout;*/
	struct hostent *hp;
	struct sockaddr_in *to;
	struct protoent *proto;
	int ch, /*fdmask, hold,*/ packlen;
	u_char *datap, *packet;
	char *target, hnamebuf[MAXHOSTNAMELEN];
	int am_i_root;

	static char *null = NULL;
	__environ = &null;
	am_i_root = (getuid()==0);
	/*
	 * Pull this stuff up front so we can drop root if desired.
	 */
	if (!(proto = getprotobyname("icmp"))) {
		(void)fprintf(stderr, "ping: unknown protocol icmp.\n");
		exit(2);
	}
	if ((s = socket(AF_INET, SOCK_RAW, proto->p_proto)) < 0) {
		if (errno==EPERM) {
			fprintf(stderr, "ping: ping must run as root\n");
		}
		else perror("ping: socket");
		exit(2);
	}

#ifdef SAFE_TO_DROP_ROOT
	setuid(getuid());
#endif
      
	
	datap = &outpack[8 + sizeof(struct timeval)];
	while ((ch = getopt(argc, argv, "I:LRc:dfh:i:l:np:qrs:t:v")) != EOF)
		switch(ch) {
		case 'c':
			npackets = atoi(optarg);
			if (npackets <= 0) {
				(void)fprintf(stderr,
				    "ping: bad number of packets to transmit.\n");
				exit(2);
			}
			break;

		
		case 's':		/* size of packet to send */
			datalen = atoi(optarg);
			if (datalen > MAXPACKET) {
				(void)fprintf(stderr,
				    "ping: packet size too large.\n");
				exit(2);
			}
			if (datalen <= 0) {
				(void)fprintf(stderr,
				    "ping: illegal packet size.\n");
				exit(2);
			}
			break;
	
		default:
			usage();
		}
	argc -= optind;
	argv += optind;
	
	if (argc == 0)
		usage();
	
	target = *argv;
        
        
	
	/* Read the output file name */	
	
	argc--;
	 
	if (argc != 1)
		nomefile="ritardi.out\0";
	else
	        nomefile=*(argv+1);    
	
	
	
	memset(&whereto, 0, sizeof(struct sockaddr));
	to = (struct sockaddr_in *)&whereto;
	to->sin_family = AF_INET;
	if (inet_aton(target, &to->sin_addr)) {
		hostname = target;
	}
	else {
		hp = gethostbyname(target);
		if (!hp) {
			(void)fprintf(stderr,
			    "ping: unknown host %s\n", target);
			exit(2);
		}
		to->sin_family = hp->h_addrtype;
		if (hp->h_length > (int)sizeof(to->sin_addr)) {
			hp->h_length = sizeof(to->sin_addr);
		}
		memcpy(&to->sin_addr, hp->h_addr, hp->h_length);
		(void)strncpy(hnamebuf, hp->h_name, sizeof(hnamebuf) - 1);
		hostname = hnamebuf;
	}

	
   
	if (datalen >= (int)sizeof(struct timeval)) /* can we time transfer */
		timing = 1;
	packlen = datalen + MAXIPLEN + MAXICMPLEN;
	packet = malloc((u_int)packlen);
	if (!packet) {
		(void)fprintf(stderr, "ping: out of memory.\n");
		exit(2);
	}
	

	ident = getpid() & 0xFFFF;
   

	if (to->sin_family == AF_INET)
		    (void)printf("PING %s (%s): %d data bytes\n", hostname,
		    inet_ntoa(*(struct in_addr *)&to->sin_addr.s_addr),
		    datalen);
	else
		(void)printf("PING %s: %d data bytes\n", hostname, datalen);
  
	(void)signal(SIGINT, finish);
	(void)signal(SIGALRM, catcher);
	
	          
	if ((options & F_FLOOD) == 0)
		catcher(0);		/* start things going */
                
	for (;;) {
		struct sockaddr_in from;
		register int cc;
		size_t fromlen;
	
		fromlen = sizeof(from);
		if ((cc = recvfrom(s, (char *)packet, packlen, 0,
		    (struct sockaddr *)&from, &fromlen)) < 0) {
			if (errno == EINTR)
				continue;
			perror("ping: recvfrom");
			continue;
		}
		pr_pack((char *)packet, cc, &from);
		if (npackets && nreceived >= npackets)
			break;
	}

	finish(0);
	/* NOTREACHED */
	return 0;
}

/*
 * catcher --
 *	This routine causes another PING to be transmitted, and then
 * schedules another SIGALRM for 1 second from now.
 * 
 * bug --
 *	Our sense of time will slowly skew (i.e., packets will not be
 * launched exactly at 1-second intervals).  This does not affect the
 * quality of the delay and loss statistics.
 */
static void
catcher(int ignore)
{
	int waittime;

	(void)ignore;
	

	pinger();
	(void)signal(SIGALRM, catcher);
	if (!npackets || ntransmitted < npackets) 
	{
	     alarm((u_int)interval);
	     
	     /* Modifico la scala dei tempi, ogni secondo diventa 10 milli secondi */
	     modifica_scala_tempi();
        }
	else 
	{
		if (nreceived) 
		{
			waittime = 2 * tmax / 1000;
			if (!waittime)
				waittime = 1;
			if (waittime > MAXWAIT)
				waittime = MAXWAIT;
		} 
		else
			waittime = MAXWAIT;
		
	        (void)signal(SIGALRM, finish);
	        (void)alarm((u_int)waittime);
	}
}

#if !defined(__GLIBC__) || (__GLIBC__ < 2)
#define icmp_type type
#define icmp_code code
#define icmp_cksum checksum
#define icmp_id un.echo.id
#define icmp_seq un.echo.sequence
#define icmp_gwaddr un.gateway
#endif /* __GLIBC__ */

#define ip_hl ihl
#define ip_v version
#define ip_tos tos
#define ip_len tot_len
#define ip_id id
#define ip_off frag_off
#define ip_ttl ttl
#define ip_p protocol
#define ip_sum check
#define ip_src saddr
#define ip_dst daddr

/*
 * pinger --
 * 	Compose and transmit an ICMP ECHO REQUEST packet.  The IP packet
 * will be added on by the kernel.  The ID field is our UNIX process ID,
 * and the sequence number is an ascending integer.  The first 8 bytes
 * of the data portion are used to hold a UNIX "timeval" struct in VAX
 * byte-order, to compute the round-trip time.
 */
static void
pinger()
{       
	register struct icmphdr *icp;
	register int cc;
	int i;
        
	/* inserisco il numero di sequenza nel campo dati*/
	*((long *)&outpack[16])=ntransmitted;
	
	icp = (struct icmphdr *)outpack;
	icp->icmp_type = ICMP_ECHO;
	icp->icmp_code = 0;
	icp->icmp_cksum = 0;
	icp->icmp_seq = ntransmitted++;		// #### Per poi discriminare a quale pacchetto si
							// riferisce il ritardo
	icp->icmp_id = ident;			/* ID */

	CLR(icp->icmp_seq % mx_dup_ck);

	if (timing)
		(void)gettimeofday((struct timeval *)&outpack[8],
		    (struct timezone *)NULL);
        
	
	
        Riempie_Partenze();
	cc = datalen + 8 ;			/* skips ICMP portion */
	

	/* compute ICMP checksum here */
	icp->icmp_cksum = in_cksum((u_short *)icp, cc);
        
	i = sendto(s, (char *)outpack, cc, 0, &whereto,
	    sizeof(struct sockaddr));
	       	   
		
	if (i < 0 || i != cc)  {
		if (i < 0)
			perror("ping: sendto");
		(void)printf("ping: wrote %s %d chars, ret=%d\n",
		    hostname, cc, i);
	}
	if (!(options & F_QUIET) && options & F_FLOOD)
		(void)write(STDOUT_FILENO, &DOT, 1);
}

/*
 * pr_pack --
 *	Print out the packet, if it came from us.  This logic is necessary
 * because ALL readers of the ICMP socket get a copy of ALL ICMP packets
 * which arrive ('tis only fair).  This permits multiple copies of this
 * program to be run without having intermingled output (or statistics!).
 */
void
pr_pack(char *buf, int cc, struct sockaddr_in *from)
{
	register struct icmphdr *icp;
	register int i;
	register u_char *cp,*dp;
/*#if 0*/
	register u_long l;
	register int j;
	static int old_rrlen;
	static char old_rr[MAX_IPOPTLEN];
/*#endif*/
	struct iphdr *ip;
	struct timeval tv, *tp;
	long triptime = 0;
	int hlen, dupflag;

	(void)gettimeofday(&tv, (struct timezone *)NULL);
	/* Check the IP header */
	ip = (struct iphdr *)buf;
	hlen = ip->ip_hl << 2;
	if (cc < datalen + ICMP_MINLEN) {
		if (options & F_VERBOSE)
			(void)fprintf(stderr,
			  "ping: packet too short (%d bytes) from %s\n", cc,
			  inet_ntoa(*(struct in_addr *)&from->sin_addr.s_addr));
		return;
	}
        
	/* Now the ICMP part */
	cc -= hlen;
	icp = (struct icmphdr *)(buf + hlen);

	if (icp->icmp_type == ICMP_ECHOREPLY) {
		if (icp->icmp_id != ident)
			return;			/* 'Twas not our ECHO */

		++nreceived;
		
		if (timing) {
#ifndef icmp_data
			tp = (struct timeval *)(icp + 1);
#else
			tp = (struct timeval *)icp->icmp_data;
#endif
                        

			tvsub(&tv, tp);
			triptime = tv.tv_sec * 10000 + (tv.tv_usec / 100);
			tsum += triptime;
			if (triptime < tmin)
				tmin = triptime;
			if (triptime > tmax)
				tmax = triptime;
			
			// #### Stampa il ritardo del pacchetto ricevuto 	
			Riempie_Ritardi( *((long *)&(icp->icmp_data[8])) , triptime );
		}

		if (TST(icp->icmp_seq % mx_dup_ck)) {
			++nrepeats;
			--nreceived;
			dupflag = 1;
		} else {
			SET(icp->icmp_seq % mx_dup_ck);
			dupflag = 0;
		}

		if (options & F_QUIET)
			return;
			
	
		if (options & F_FLOOD)
			(void)write(STDOUT_FILENO, &BSPACE, 1);
		else {
			(void)printf("%d bytes from %s: icmp_seq=%u", cc,
			   inet_ntoa(*(struct in_addr *)&from->sin_addr.s_addr),
			   icp->icmp_seq);
			(void)printf(" ttl=%d", ip->ip_ttl);
			
			if (timing)
				(void)printf(" time=%ld.%ld ms\n", triptime/10, triptime%10);
			if (dupflag)
				(void)printf(" (DUP!)");
		
			/* check the data */
/*#ifndef icmp_data
			cp = ((u_char*)(icp + 1) + 12);
#else
			cp = (u_char*)icp->icmp_data +12;
#endif
			dp = &outpack[8 + sizeof(struct timeval)+4];
			for (i = 8; i < datalen; ++i, ++cp, ++dp)
			         {
				  if (*cp != *dp) 
				  {
	                                 (void)printf("\nwrong data byte #%d should be 0x%x but was 0x%x",
	                                   i, *dp, *cp);
					cp = (u_char*)(icp + 1);
					for (i = 8; i < datalen; ++i, ++cp) 
					{
						if ((i % 32) == 8)
							(void)printf("\n\t");
						(void)printf("%x ", *cp);
					}
					break;
			          }
			         }*/
		}
	} 
	
/*#if 0*/
	/* Display any IP options */

	cp = (u_char *)buf + sizeof(struct iphdr);

	for (; hlen > (int)sizeof(struct iphdr); --hlen, ++cp)
		switch (*cp) {
		case IPOPT_EOL:
			hlen = 0;
			break;
		case IPOPT_LSRR:
			(void)printf("\nLSRR: ");
			hlen -= 2;
			j = *++cp;
			++cp;
			if (j > IPOPT_MINOFF)
				for (;;) {
					l = *++cp;
					l = (l<<8) + *++cp;
					l = (l<<8) + *++cp;
					l = (l<<8) + *++cp;
					if (l == 0)
						(void)printf("\t0.0.0.0");
				else
					(void)printf("\t%s", pr_addr(ntohl(l)));
				hlen -= 4;
				j -= 4;
				if (j <= IPOPT_MINOFF)
					break;
				(void)putchar('\n');
			}
			break;
		case IPOPT_RR:
			j = *++cp;		/* get length */
			i = *++cp;		/* and pointer */
			hlen -= 2;
			if (i > j)
				i = j;
			i -= IPOPT_MINOFF;
			if (i <= 0)
				continue;
			if (i == old_rrlen
			    && cp == (u_char *)buf + sizeof(struct iphdr) + 2
			    && !memcmp((char *)cp, old_rr, i)
			    && !(options & F_FLOOD)) {
				(void)printf("\t(same route)");
				i = ((i + 3) / 4) * 4;
				hlen -= i;
				cp += i;
				break;
			}
			old_rrlen = i;
			memcpy(old_rr, cp, i);
			(void)printf("\nRR: ");
			for (;;) {
				l = *++cp;
				l = (l<<8) + *++cp;
				l = (l<<8) + *++cp;
				l = (l<<8) + *++cp;
				if (l == 0)
					(void)printf("\t0.0.0.0");
				else
					(void)printf("\t%s", pr_addr(ntohl(l)));
				hlen -= 4;
				i -= 4;
				if (i <= 0)
					break;
				(void)putchar('\n');
			}
			break;
		case IPOPT_NOP:
			(void)printf("\nNOP");
			break;
		default:
			(void)printf("\nunknown option %x", *cp);
			break;
		}
/*#endif*/
	if (!(options & F_FLOOD)) {
		/*(void)putchar('\n');*/
		(void)fflush(stdout);
	}
}

/*
 * in_cksum --
 *	Checksum routine for Internet Protocol family headers (C Version)
 */
static int
in_cksum(u_short *addr, int len)
{
	register int nleft = len;
	register u_short *w = addr;
	register int sum = 0;
	u_short answer = 0;

	/*
	 * Our algorithm is simple, using a 32 bit accumulator (sum), we add
	 * sequential 16 bit words to it, and at the end, fold back all the
	 * carry bits from the top 16 bits into the lower 16 bits.
	 */
	while (nleft > 1)  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if (nleft == 1) {
		*(u_char *)(&answer) = *(u_char *)w ;
		sum += answer;
	}

	/* add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return(answer);
}

/*
 * tvsub --
 *	Subtract 2 timeval structs:  out = out - in.  Out is assumed to
 * be >= in.
 */
static void
tvsub(register struct timeval *out, register struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) < 0) {
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

/*
 * finish --
 *	Print out statistics, and give up.
 */
static void
finish(int ignore)
{
	(void)ignore;

	(void)signal(SIGINT, SIG_IGN);
	(void)putchar('\n');
	(void)fflush(stdout);
	(void)printf("--- %s ping statistics ---\n", hostname);
	(void)printf("%ld packets transmitted, ", ntransmitted);
	(void)printf("%ld packets received, ", nreceived);
	if (nrepeats)
		(void)printf("+%ld duplicates, ", nrepeats);
	if (ntransmitted) {
		if (nreceived > ntransmitted) {
			(void)printf("-- somebody's printing up packets!");
		} else {
			(void)printf("%d%% packet loss",
			    (int) (((ntransmitted - nreceived) * 100) /
			    ntransmitted));
		}
	}
	(void)putchar('\n');
	if (nreceived && timing)
		(void)printf("round-trip min/avg/max = %ld.%ld/%lu.%ld/%ld.%ld ms\n",
			tmin/10, tmin%10,
			(tsum / (nreceived + nrepeats))/10,
			(tsum / (nreceived + nrepeats))%10,
			tmax/10, tmax%10);

	if (nreceived==0) exit(1);
	
	// #### Stampo le statistiche real time
	Stampa();
	
	exit(0);
}

#ifdef notdef
static char *ttab[] = {
	"Echo Reply",		/* ip + seq + udata */
	"Dest Unreachable",	/* net, host, proto, port, frag, sr + IP */
	"Source Quench",	/* IP */
	"Redirect",		/* redirect type, gateway, + IP  */
	"Echo",
	"Time Exceeded",	/* transit, frag reassem + IP */
	"Parameter Problem",	/* pointer + IP */
	"Timestamp",		/* id + seq + three timestamps */
	"Timestamp Reply",	/* " */
	"Info Request",		/* id + sq */
	"Info Reply"		/* " */
};
#endif



/*
 * pr_addr --
 *	Return an ascii host address as a dotted quad and optionally with
 * a hostname.
 */
static char *
pr_addr(u_long l)
{
	struct hostent *hp;
	static char buf[256];

	if ((options & F_NUMERIC) ||
	    !(hp = gethostbyaddr((char *)&l, 4, AF_INET)))
		(void)snprintf(buf, sizeof(buf), "%s", 
			       inet_ntoa(*(struct in_addr *)&l));
	else
		(void)snprintf(buf, sizeof(buf), "%s (%s)", hp->h_name,
		    inet_ntoa(*(struct in_addr *)&l));
	return(buf);
}



static void
usage(void)
{
	(void)fprintf(stderr,
	    "Usage: fastping  [-c count]   [-s packetsize]  host [filename]\n");
	exit(2);
}

// #####       P I N G       S O U R C E    E N D  ######################

// Inizio delle procedure aggiunte al sorgente del ping.

// #### Modifico la scala dei tempi, ogni secondo diventa 10 milli secondi

void modifica_scala_tempi(void)
{
	struct itimerval value;
	int which = ITIMER_REAL; // Usato dalla SIGALRM
	
	getitimer(which, &value);
 
	value.it_value.tv_usec = INTERVALLO /* value.it_value.tv_sec*/;  // Ogni secondo diventa 10 milli secondi
	value.it_value.tv_sec = 0;
       
	setitimer(which, &value, NULL);
}


// Stampa un messaggio di warning o di errore
void errore ( short livello, char *str, short linea )
{
	printf("ERROR in flowping.c line %d\n", linea);
	printf("       description:\n");
	printf("       %s\n", str);
	if ( livello == FATAL) 
	{
		exit(2);
	}
}


/* Inizializza il vettore dei round trip delay */

void Inizializza()
{
 int i;
 for (i=0;i<MAX_PAC;i++)
 ritardi[i]=0;
}
 


/*  Inserisce nel vettore ritardi il RTD di ogni pacchetto*/

void Riempie_Ritardi ( long indice, long ritardo )
{ 
  /* controllo se e' presente un fuori sequenza */
  if ( indice < prec)
  {
  fuoriseq++;
  //printf("%ld\n",fuoriseq);

  }
  
  prec=indice;
  
  //printf("%ld\n",indice);
  ritardi[indice]=ritardo;
  
 
}
	


/* Viene invocata per inserire nel vettore partenze l'istante di partenza */

void Riempie_Partenze()
{  
 (void)gettimeofday(&partenze[ntransmitted-1],(struct timezone *)NULL);  
} 

/* Viene invocata per calcolare le statistiche sui tempi di interpartenze */

/*struct Stat_Part Statistiche_Partenze()
{
  struct Stat_Part tmp;
  double f,fprec=0,media=0,varianza=0,deviazione=0;
  long i=0;
  FILE *infile;
  
  infile=fopen("partenze.out","r");
  
  /* Calcolo il valore medio delle interpartenze */
  /*if (infile==NULL)
       exit(1);
  while (!feof(infile))
  {  
   fscanf(infile,"%lf",&f);
   fscanf(infile,"\n");
   if (i>0)
       media+=f-fprec;
   fprec=f;
   i++;
  }  
  media=media/(i-1);
  fclose(infile);
  infile=fopen("partenze.out","r");
  
  /*Calcolo la deviazione standard dell'intervallo di interpartenza */
  /*if (infile==NULL)
       exit(1);
  i=0; 
  while (!feof(infile))
  {  
   fscanf(infile,"%lf",&f);
   fscanf(infile,"\n");
   if (i>0)
    {       
        deviazione=f-fprec-media;
        varianza+=deviazione*deviazione;       
    }    
   fprec=f;
   i++;
  }  
  varianza=varianza/(i-1);
  deviazione=sqrt(varianza);
  tmp.media=media;
  tmp.deviazione=deviazione;
  fclose(infile);
  return tmp;
} */ 

/* Stampa nei files i vettori */ 

void Stampa()
{
 time_t tp;
 char *data;
 int i=0; 
 double p;
/* struct Stat_Part sp;*/
 FILE *outfile;
 
 /* Stampo gli istanti di partenza in millisecondi */
 
 outfile=fopen("partenze.out","w");
 if (outfile == NULL)
 {      errore(FATAL,"Errore apertura file partenze.out",__LINE__);
	exit(1);
 }
 for(i=0;i<npackets;i++)
 {  
   p = (partenze[i].tv_sec%1000000)*1000;
   p=p+(float)partenze[i].tv_usec/1000;  /*tutto espresso in millisecondi */  
   fprintf(outfile,"%f\n",p);
 }
  
 fclose(outfile);  

 /* Stampo i round trip delay in millisecondi e in testa al file le statistiche sui*/
 /* pacchetti duplicati e persi */
 
 /*sp=Statistiche_Partenze();*/
 outfile=fopen(nomefile,"w");
 if (outfile == NULL)
 {
            errore(FATAL,"Errore apertura file di output",__LINE__);
	    exit(1);
 }
 
 /*Prelevo la data e l'ora */  
 time(&tp);
 data=ctime(&tp);
/*Stampo le statistica in testa al file */
 fprintf(outfile,"#Date                               : %s",data);
 fprintf(outfile,"#Destination host                   : %s\n",hostname);
 fprintf(outfile,"#Packet length (data)               : %d byte\n",datalen);
 fprintf(outfile,"#Interdeparture time                : %d ms\n",INTERVALLO/1000);
/* fprintf(outfile,"#Media Intervalli Interpartenza   : %f ms\n",sp.media);
 fprintf(outfile,"#Deviazione Intervalli Interpartenza  : %f ms\n",sp.deviazione);*/
 fprintf(outfile,"#Packets Transmitted                : %ld\n",npackets);
 fprintf(outfile,"#Loss Packets                       : %ld\n",npackets-nreceived);
 fprintf(outfile,"#Duplicated Packets                 : %ld\n",nrepeats);
 fprintf(outfile,"#Out of order packets               : %ld\n\n",fuoriseq);
 fprintf(outfile,"#Round Trip delays (ms) (0.0==loss)\n\n");
 
 /*Stampo i ritardi */
 for(i=0;i<npackets;i++)
 {
           fprintf(outfile,"%ld.%ld\n",ritardi[i]/10,ritardi[i]%10);
 }
 fclose(outfile);  
}
