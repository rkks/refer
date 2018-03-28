/*
	Socket options example
	AUP2, Sec. 8.03

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "defs.h"
#ifdef SOLARIS
#define __EXTENSIONS__
#endif
#include <sys/socket.h>
#undef __EXTENSIONS__
#include <netinet/in.h>
#include <netinet/tcp.h>
/*[show]*/
typedef enum {OT_INT, OT_LINGER, OT_TIMEVAL} OPT_TYPE;

static void show(int skt, int level, int option, const char *name,
  OPT_TYPE type)
{
	socklen_t len;
	int n;
	struct linger lng;
	struct timeval tv;

	switch (type) {
	case OT_INT:
		len = sizeof(n);
		if (getsockopt(skt, level, option, &n, &len) == -1)
			printf("%s FAILED (%s)\n", name, strerror(errno));
		else
			printf("%s = %d\n", name, n);
		break;
	case OT_LINGER:
		len = sizeof(lng);
		if (getsockopt(skt, level, option, &lng, &len) == -1)
			printf("%s FAILED (%s)\n", name, strerror(errno));
		else
			printf("%s = l_onoff: %d; l_linger: %d secs.\n", name,
			  lng.l_onoff, lng.l_linger);
		break;
	case OT_TIMEVAL:
		len = sizeof(tv);
		if (getsockopt(skt, level, option, &tv, &len) == -1)
			printf("%s FAILED (%s)\n", name, strerror(errno));
		else
			printf("%s = %ld secs.; %ld usecs.\n", name,
			  (long)tv.tv_sec, (long)tv.tv_usec);
	}
}

static void showall(int skt, const char *caption)
{
	printf("\n%s\n", caption);
	show(skt, SOL_SOCKET, SO_ACCEPTCONN, "SO_ACCEPTCONN", OT_INT);
	show(skt, SOL_SOCKET, SO_BROADCAST, "SO_BROADCAST", OT_INT);
	show(skt, SOL_SOCKET, SO_DEBUG, "SO_DEBUG", OT_INT);
	show(skt, SOL_SOCKET, SO_DONTROUTE, "SO_DONTROUTE", OT_INT);
	show(skt, SOL_SOCKET, SO_ERROR, "SO_ERROR", OT_INT);
	show(skt, SOL_SOCKET, SO_KEEPALIVE, "SO_KEEPALIVE", OT_INT);
	show(skt, SOL_SOCKET, SO_LINGER, "SO_LINGER", OT_LINGER);
	show(skt, SOL_SOCKET, SO_OOBINLINE, "SO_OOBINLINE", OT_INT);
	show(skt, SOL_SOCKET, SO_RCVBUF, "SO_RCVBUF", OT_INT);
	show(skt, SOL_SOCKET, SO_RCVLOWAT, "SO_RCVLOWAT", OT_INT);
	show(skt, SOL_SOCKET, SO_RCVTIMEO, "SO_RCVTIMEO", OT_TIMEVAL);
	show(skt, SOL_SOCKET, SO_REUSEADDR, "SO_REUSEADDR", OT_INT);
	show(skt, SOL_SOCKET, SO_SNDBUF, "SO_SNDBUF", OT_INT);
	show(skt, SOL_SOCKET, SO_SNDLOWAT, "SO_SNDLOWAT", OT_INT);
	show(skt, SOL_SOCKET, SO_SNDTIMEO, "SO_SNDTIMEO", OT_TIMEVAL);
	show(skt, SOL_SOCKET, SO_TYPE, "SO_TYPE", OT_INT);
/*
	show(skt, IPPROTO_TCP, TCP_NODELAY, "TCP_NODELAY", OT_INT);
	show(skt, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, "IPV6_MULTICAST_HOPS",
	  OT_INT);
	show(skt, IPPROTO_IPV6, IPV6_MULTICAST_IF, "IPV6_MULTICAST_IF",
	  OT_INT);
	show(skt, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, "IPV6_MULTICAST_LOOP",
	  OT_INT);
	show(skt, IPPROTO_IPV6, IPV6_UNICAST_HOPS, "IPV6_UNICAST_HOPS",
	  OT_INT);
*/
}

int main(void)
{
	int skt;

	ec_neg1( skt = socket(AF_INET, SOCK_STREAM, 0) )
	showall(skt, "AF_INET SOCK_STREAM");
	ec_neg1( close(skt) )
	ec_neg1( skt = socket(AF_INET, SOCK_DGRAM, 0) )
	showall(skt, "AF_INET SOCK_DGRAM");
	ec_neg1( close(skt) )
/*
	ec_neg1( skt = socket(AF_INET6, SOCK_STREAM, 0) )
	showall(skt, "AF_INET6 SOCK_STREAM");
	ec_neg1( close(skt) )
*/
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
