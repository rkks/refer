/*
	SOCK_DGRAM example program (multiple clients)
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
#define SOCKETNAME_SERVER "SktOne"
#define SOCKETNAME_CLIENT "SktTwo"

static struct sockaddr_un sa_server;

#define MSG_SIZE 100

static void run_client(int nclient)
{
	struct sockaddr_un sa_client;
	int fd_skt;
	ssize_t nrecv;
	char msg[MSG_SIZE];
	int i;

	if (fork() == 0) { /* client */
		sleep(1); /* let server startup first */
		ec_neg1( fd_skt = socket(AF_UNIX, SOCK_DGRAM, 0) )
		snprintf(sa_client.sun_path, sizeof(sa_client.sun_path),
		  "%s-%d", SOCKETNAME_CLIENT, nclient);
		(void)unlink(sa_client.sun_path);
		sa_client.sun_family = AF_UNIX;
		ec_neg1( bind(fd_skt, (struct sockaddr *)&sa_client,
		  sizeof(sa_client)) )
		for (i = 1; i <= 4; i++) {
			snprintf(msg, sizeof(msg), "Message #%d", i);
			ec_neg1( sendto(fd_skt, msg, sizeof(msg), 0,
			  (struct sockaddr *)&sa_server, sizeof(sa_server)) )
			ec_neg1( nrecv = read(fd_skt, msg, sizeof(msg)) )
			if (nrecv != sizeof(msg)) {
				printf("client got short message\n");
				break;
			}
			printf("Got \"%s\" back\n", msg);
		}
		ec_neg1( close(fd_skt) )
		exit(EXIT_SUCCESS);
	}
	return;

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}

static void run_server(void)
{
	int fd_skt;
	ssize_t nrecv;
	char msg[MSG_SIZE];
	struct sockaddr_storage sa;
	socklen_t sa_len;

	ec_neg1( fd_skt = socket(AF_UNIX, SOCK_DGRAM, 0) )
	ec_neg1( bind(fd_skt, (struct sockaddr *)&sa_server, sizeof(sa_server)) )
	while (true) {
		sa_len = sizeof(sa);
		ec_neg1( nrecv = recvfrom(fd_skt, msg, sizeof(msg), 0,
		  (struct sockaddr *)&sa, &sa_len) )
		if (nrecv != sizeof(msg)) {
			printf("server got short message\n");
			break;
		}
		msg[0] = 'm';
		ec_neg1( sendto(fd_skt, msg, sizeof(msg), 0,
		  (struct sockaddr *)&sa, sa_len) )
	}
	ec_neg1( close(fd_skt) )
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}

int main(void)
{
	int nclient;

	strcpy(sa_server.sun_path, SOCKETNAME_SERVER);
	sa_server.sun_family = AF_UNIX;
	(void)unlink(SOCKETNAME_SERVER);
	for (nclient = 1; nclient <= 3; nclient++)
		run_client(nclient);
	run_server();
	exit(EXIT_SUCCESS);
}
