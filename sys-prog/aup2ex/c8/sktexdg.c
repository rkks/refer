/*
	SOCK_DGRAM example program
	AUP2, Sec. 8.06.2

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
#include <sys/un.h>
/*[pgm]*/
#define SOCKETNAME1 "SktOne"
#define SOCKETNAME2 "SktTwo"

#define MSG_SIZE 100

int main(void)
{
	struct sockaddr_un sa1, sa2;

	strcpy(sa1.sun_path, SOCKETNAME1);
	sa1.sun_family = AF_UNIX;
	strcpy(sa2.sun_path, SOCKETNAME2);
	sa2.sun_family = AF_UNIX;
	(void)unlink(SOCKETNAME1);
	(void)unlink(SOCKETNAME2);
	if (fork() == 0) { /* Peer 1 */
		int fd_skt;
		ssize_t nread;
		char msg[MSG_SIZE];
		int i;

		sleep(1); /* let peer 2 startup first */
		ec_neg1( fd_skt = socket(AF_UNIX, SOCK_DGRAM, 0) )
		ec_neg1( bind(fd_skt, (struct sockaddr *)&sa1, sizeof(sa1)) )
		for (i = 1; i <= 4; i++) {
			snprintf(msg, sizeof(msg), "Message #%d", i);
			ec_neg1( sendto(fd_skt, msg, sizeof(msg), 0,
			  (struct sockaddr *)&sa2, sizeof(sa2)) )
			ec_neg1( nread = read(fd_skt, msg, sizeof(msg)) )
			if (nread != sizeof(msg)) {
				printf("Peer 1 got short message\n");
				break;
			}
			printf("Got \"%s\" back\n", msg);
		}
		ec_neg1( close(fd_skt) )
		exit(EXIT_SUCCESS);
	}
	else { /* Peer 2 */
		int fd_skt;
		ssize_t nread;
		char msg[MSG_SIZE];

		ec_neg1( fd_skt = socket(AF_UNIX, SOCK_DGRAM, 0) )
		ec_neg1( bind(fd_skt, (struct sockaddr *)&sa2, sizeof(sa2)) )
		while (true) {
			ec_neg1( nread = read(fd_skt, msg, sizeof(msg)) )
			if (nread != sizeof(msg)) {
				printf("Peer 2 got short message\n");
				break;
			}
			msg[0] = 'm';
			ec_neg1( sendto(fd_skt, msg, sizeof(msg), 0,
			  (struct sockaddr *)&sa1, sizeof(sa1)) )
		}
		ec_neg1( close(fd_skt) )
		exit(EXIT_SUCCESS);
	}

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
