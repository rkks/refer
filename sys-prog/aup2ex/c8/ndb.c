/*
	Network database examples
	AUP2, Sec. 8.08

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
#include <netdb.h>
#undef __EXTENSIONS__
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

/*[ifdb]*/
static void ifdb(void)
{
	struct if_nameindex *ni;
	int i;

	ec_null( ni = if_nameindex() )
	for (i = 0; ni[i].if_index != 0 || ni[i].if_name != NULL; i++)
		printf("index: %d; name: %s\n", ni[i].if_index, ni[i].if_name);
	if_freenameindex(ni);
	return;

EC_CLEANUP_BGN
	EC_FLUSH("ifdb")
EC_CLEANUP_END
}
/*[display_hostent]*/
static void display_hostent(struct hostent *h)
{
	int i;

	printf("name: %s; type: %d; len: %d\n", h->h_name, h->h_addrtype,
	  h->h_length);
	for (i = 0; h->h_aliases[i] != NULL; i++)
		printf("\t%s\n", h->h_aliases[i]);
	if (h->h_addrtype == AF_INET) {
		for (i = 0; h->h_addr_list[i] != NULL; i++)
			printf("\t%s\n",
			  inet_ntoa(*((struct in_addr *)h->h_addr_list[i])));
	}
}
/*[hostdb]*/
static void hostdb(void)
{
	struct hostent *h;

	sethostent(true);
	while ((h = gethostent()) != NULL)
		display_hostent(h);
	endhostent();
}
/*[gethostbyname_ex]*/
static void gethostbyname_ex(void)
{
	struct hostent *h;

	if ((h = gethostbyname("www.yahoo.com")) == NULL) {
		if (h_errno == HOST_NOT_FOUND)
			printf("host not found\n");
		else
			printf("h_errno = %d\n", h_errno);
	}
	else
		display_hostent(h);
}
/*[gethostbyaddr_ex]*/
static void gethostbyaddr_ex(void)
{
	struct hostent *h;
	in_addr_t a;

	ec_neg1( a = inet_addr("66.218.71.94") )
	/*
		Solaris wants a "const char *" as first arg of gethostbyaddr,
		but SUS says "const void *" so we will put in a cast.
	*/
	if ((h = gethostbyaddr((const char *)&a, sizeof(a), AF_INET))
	  == NULL) {
		if (h_errno == HOST_NOT_FOUND)
			printf("address not found\n");
		else
			printf("h_errno = %d\n", h_errno);
	}
	else
		display_hostent(h);
	return;

EC_CLEANUP_BGN
	EC_FLUSH("gethostbyaddr_ex")
EC_CLEANUP_END
}
/*[getnameinfo_ex]*/
static void getnameinfo_ex(void)
{
    struct sockaddr_in sa;
    char nodename[200], servname[200];

    sa.sin_family = AF_INET;
    sa.sin_port = htons(80);
    sa.sin_addr.s_addr =  inet_addr("216.109.125.70");
	ec_ai( getnameinfo((struct sockaddr *)&sa, sizeof(sa), nodename,
	  sizeof(nodename), servname, sizeof(servname), 0) )
	printf("node: %s; service: %s\n", nodename, servname);
	return;

EC_CLEANUP_BGN
	EC_FLUSH("getnameinfo_ex")
EC_CLEANUP_END
}
/*[display_netent]*/
static void display_netent(struct netent *n)
{
	int i;

	printf("name: %s; type: %d; number: %lu\n", n->n_name, n->n_addrtype,
	  (unsigned long)n->n_net);
	for (i = 0; n->n_aliases[i] != NULL; i++)
		printf("\t%s\n", n->n_aliases[i]);
}
/*[netdb]*/
static void netdb(void)
{
	struct netent *n;

	setnetent(true);
	while ((n = getnetent()) != NULL)
		display_netent(n);
	endnetent();
}
/*[display_protoent]*/
static void display_protoent(struct protoent *p)
{
	int i;

	printf("name: %s; number: %d\n", p->p_name, p->p_proto);
	for (i = 0; p->p_aliases[i] != NULL; i++)
		printf("\t%s\n", p->p_aliases[i]);
}
/*[protodb]*/
static void protodb(void)
{
	struct protoent *p;

	setprotoent(true);
	while ((p = getprotoent()) != NULL)
		display_protoent(p);
	endprotoent();
}
/*[display_servent]*/
static void display_servent(struct servent *s)
{
	int i;

	printf("name: %s; port: %d; protocol: %s\n", s->s_name, s->s_port,
	  s->s_proto);
	for (i = 0; s->s_aliases[i] != NULL; i++)
		printf("\t%s\n", s->s_aliases[i]);
}
/*[servdb]*/
static void servdb(void)
{
	struct servent *s;

	setservent(true);
	while ((s = getservent()) != NULL)
		display_servent(s);
	endservent();
}
/*[cvt]*/
static void cvt(void)
{
	char ipv6[16], ipv6str[INET6_ADDRSTRLEN], ipv4str[INET_ADDRSTRLEN];
	uint32_t ipv4;
	int r;

	ec_neg1( r = inet_pton(AF_INET, "66.218.71.94", &ipv4) )
	if (r == 0)
		printf("Can't convert\n");
	else {
		ec_null( inet_ntop(AF_INET, &ipv4, ipv4str, sizeof(ipv4str)) )
		printf("%s\n", ipv4str);
	}
	ec_neg1( r = inet_pton(AF_INET6,
	  "FEDC:BA98:7654:3210:FEDC:BA98:7654:3210", &ipv6) )
	if (r == 0)
		printf("Can't convert\n");
	else {
		ec_null( inet_ntop(AF_INET6, &ipv6, ipv6str, sizeof(ipv6str)) )
		printf("%s\n", ipv6str);
	}
	return;

EC_CLEANUP_BGN
	EC_FLUSH("cvt")
EC_CLEANUP_END
}
/*[]*/
int main(void)
{
#if 0
	struct in_addr in;

	in.s_addr = (in_addr_t)gethostid();
	printf("hostid: %ld %lx %s\n", gethostid(), gethostid(),
	  inet_ntoa(in));
	hostdb();
	gethostbyaddr_ex();
	gethostbyname_ex();
	getnameinfo_ex();
	netdb();
	protodb();
	servdb();
#endif
	ifdb();
	cvt();
	exit(EXIT_SUCCESS);
}
