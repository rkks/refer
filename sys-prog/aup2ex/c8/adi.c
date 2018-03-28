/**
	getaddrinfo test program
	AUP2, Sec. 8.02.6

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
/*[]*/
static void a1(void)
{
	struct addrinfo *infop = NULL;
	int r;

	r = getaddrinfo("www.basepath.com", NULL, NULL, &infop);
	if (r != 0) {
		printf("Got error number %d\n", r);
		EC_FAIL
	}
	printf("It worked!\n");
	return;

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
#define main main1
/*[main1]*/
int main(void)
{
	struct addrinfo *infop = NULL, hint;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	ec_ai( getaddrinfo("www.yahoo.com", "80", &hint, &infop) )
	for ( ; infop != NULL; infop = infop->ai_next) {
		struct sockaddr_in *sa = (struct sockaddr_in *)infop->ai_addr;

		printf("%s port: %d protocol: %d\n", inet_ntoa(sa->sin_addr),
		  ntohs(sa->sin_port), infop->ai_protocol);
	}
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
#undef main
static void a3(void)
{
	struct addrinfo *infop = NULL, *a, hint;
	int r;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET6;
	hint.ai_socktype = SOCK_STREAM;
	//r = getaddrinfo("2001:0388::", "32", &hint, &infop);
	r = getaddrinfo("ipv6.research.microsoft.com", NULL, &hint, &infop);
	if (r != 0) {
		printf("Got error number %d\n", r);
		printf("%s\n", gai_strerror(r));
		EC_FAIL
	}
	printf("It worked!\n");
	for (a = infop; a != NULL; a = a->ai_next) {
		unsigned long ip = (long)((struct sockaddr_in *)a->ai_addr)->sin_addr.s_addr;

		if (a->ai_family == AF_INET6)
			printf("Bingo!\n");
		printf("port = 0x%x (%d) {%d,%d,%d} ip=0x%lx\n",
		  ((struct sockaddr_in *)a->ai_addr)->sin_port,
		  ((struct sockaddr_in *)a->ai_addr)->sin_port,
		  a->ai_family, a->ai_socktype, a->ai_protocol, ip);
		printf("\t\t%lu.%lu.%lu.%lu\n", ip & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, ip >> 24);
	}
	return;

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
//#define main main2
/*[main2]*/
#define REQUEST "GET / HTTP/1.0\r\n\r\n"

int main(void)
{
	struct addrinfo *infop = NULL, hint;
	int fd_skt;
	char buf[1000];
	ssize_t nread;

printf("%s\n", strerror(EAI_ADDRFAMILY));
printf("%s\n", strerror(EAI_AGAIN));
printf("%s\n", strerror(EAI_BADFLAGS));
printf("%s\n", strerror(EAI_FAIL));
exit(EXIT_SUCCESS);

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	ec_ai( getaddrinfo("www.yahoo.com", "80", &hint, &infop) )
	ec_neg1( fd_skt = socket(infop->ai_family, infop->ai_socktype,
	  infop->ai_protocol) )
	ec_neg1( connect(fd_skt, (struct sockaddr *)infop->ai_addr,
	  infop->ai_addrlen) )
	ec_neg1( write(fd_skt, REQUEST, strlen(REQUEST) ) )
	ec_neg1( nread = read(fd_skt, buf, sizeof(buf)) )
	(void)write(STDOUT_FILENO, buf, nread);
	ec_neg1( close(fd_skt) )
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
#if 0
int main(void)
{
	a3();
	exit(EXIT_SUCCESS);
}
#endif
/*[]*/
