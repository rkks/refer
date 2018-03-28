/*
	inet_addr example
	AUP2, Sec. 8.02.3

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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/*[pgm]*/
#define REQUEST "GET / HTTP/1.0\r\n\r\n"

int main(void)
{
	struct sockaddr_in sa;
	int fd_skt;
	char buf[1000];
	ssize_t nread;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(80);
	sa.sin_addr.s_addr =  inet_addr("216.109.125.70");
	ec_neg1( fd_skt = socket(AF_INET, SOCK_STREAM, 0) )
	ec_neg1( connect(fd_skt, (struct sockaddr *)&sa, sizeof(sa)) )
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

int main1(void)
{
	struct sockaddr_in sa;
	int fd_skt;
	char buf[1000];
	ssize_t nread;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(80);
	sa.sin_addr.s_addr =  htonl((216UL << 24) + (109UL << 16) +
	  (125UL << 8) + 70UL); /* 216.109.125.70 */
printf("%ld %lx\n", (long)sa.sin_addr.s_addr, (long)sa.sin_addr.s_addr);
	ec_neg1( fd_skt = socket(AF_INET, SOCK_STREAM, 0) )
	ec_neg1( connect(fd_skt, (struct sockaddr *)&sa, sizeof(sa)) )
	ec_neg1( write(fd_skt, REQUEST, strlen(REQUEST) ) )
	ec_neg1( nread = read(fd_skt, buf, sizeof(buf)) )
	(void)write(STDOUT_FILENO, buf, nread);
	ec_neg1( close(fd_skt) )
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
