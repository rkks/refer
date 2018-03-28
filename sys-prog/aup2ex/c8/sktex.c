/*
	Socket example program
	AUP2, Sec. 8.01.1

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
#include <sys/un.h>
/*[pgm]*/
#define SOCKETNAME "MySocket"

int main(void)
{
	struct sockaddr_un sa;

	(void)unlink(SOCKETNAME);
	strcpy(sa.sun_path, SOCKETNAME);
	sa.sun_family = AF_UNIX;
	if (fork() == 0) { /* client */
		int fd_skt;
		char buf[100];

		ec_neg1( fd_skt = socket(AF_UNIX, SOCK_STREAM, 0) )
		while (connect(fd_skt, (struct sockaddr *)&sa, sizeof(sa)) == -1)
			if (errno == ENOENT) {
				sleep(1);
				continue;
			}
			else
				EC_FAIL
		ec_neg1( write(fd_skt, "Hello!", 7 ) )
		ec_neg1( read(fd_skt, buf, sizeof(buf)) )
		printf("Client got \"%s\"\n", buf);
		ec_neg1( close(fd_skt) )
		exit(EXIT_SUCCESS);
	}
	else { /* server */
		int fd_skt, fd_client;
		char buf[100];

		ec_neg1( fd_skt = socket(AF_UNIX, SOCK_STREAM, 0) )
		ec_neg1( bind(fd_skt, (struct sockaddr *)&sa, sizeof(sa)) )
		ec_neg1( listen(fd_skt, SOMAXCONN) )
		ec_neg1( fd_client = accept(fd_skt, NULL, 0) )
		ec_neg1( read(fd_client, buf, sizeof(buf)) )
		printf("Server got \"%s\"\n", buf);
		ec_neg1( write(fd_client, "Goodbye!", 9 ) )
		ec_neg1( close(fd_skt) )
		ec_neg1( close(fd_client) )
		exit(EXIT_SUCCESS);
	}

EC_CLEANUP_BGN
		exit(EXIT_FAILURE);
EC_CLEANUP_END
}
/*[]*/
