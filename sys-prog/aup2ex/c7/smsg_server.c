/*
	FIFO-based server
	AUP2, Sec. 7.02.2

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
#include "smsg.h"

#define WANT_BUGFIX

/* Server */
/* Assumes no partial reads */
/*[main]*/
int main(void)
{
	int fd_server, fd_client, i;
#ifdef WANT_BUGFIX
	int fd_server_w;
#endif
	ssize_t nread;
	struct simple_message msg;
	char fifo_name[100];

	printf("server started\n");
	if (mkfifo(SERVER_FIFO_NAME, PERM_FILE) == -1 && errno != EEXIST)
		EC_FAIL
	// will block waiting for writer
	ec_neg1( fd_server = open(SERVER_FIFO_NAME, O_RDONLY) )
#ifdef WANT_BUGFIX
	ec_neg1( fd_server_w = open(SERVER_FIFO_NAME, O_WRONLY) )
#endif
	while (true) {
		/* will return EOF if no writers -- a bug! */
		ec_neg1( nread = read(fd_server, &msg, sizeof(msg)) )
		//printf("back from read; nread = %d\n", nread);
		if (nread == 0) {
			errno = ENETDOWN;
			EC_FAIL
		}
		for (i = 0; msg.sm_data[i] != '\0'; i++)
			msg.sm_data[i] = toupper(msg.sm_data[i]);
		ec_false( make_fifo_name(msg.sm_clientpid, fifo_name,
		  sizeof(fifo_name)) )
		ec_neg1( fd_client = open(fifo_name, O_WRONLY) )
		ec_neg1( write(fd_client, &msg, sizeof(msg)) )
		ec_neg1( close(fd_client) )
	}
	/* never actually get here */
	ec_neg1( close(fd_server) )
#ifdef WANT_BUGFIX
	ec_neg1( close(fd_server_w) )
#endif
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
