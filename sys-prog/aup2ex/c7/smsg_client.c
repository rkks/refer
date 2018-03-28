/*
	FIFO-based client
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

/* Client */
/* Assumes no partial reads */

/*[main]*/
int main(int argc, char *argv[])
{
	int fd_server, fd_client = -1, i;
#ifdef WANT_BUGFIX
	int fd_client_w = -1;
#endif
	ssize_t nread;
	struct simple_message msg;
	char fifo_name[100];
	char *work[] = {
		"applesauce",
		"tiger",
		"mountain",
		NULL
	};

	printf("client %ld started\n", (long)getpid());
	msg.sm_clientpid = getpid();
	ec_false( make_fifo_name(msg.sm_clientpid, fifo_name,
	  sizeof(fifo_name)) )
	if (mkfifo(fifo_name, PERM_FILE) == -1 && errno != EEXIST)
		EC_FAIL
	ec_neg1( fd_server = open(SERVER_FIFO_NAME, O_WRONLY) )
	for (i = 0; work[i] != NULL; i++) {
		strcpy(msg.sm_data, work[i]);
		ec_neg1( write(fd_server, &msg, sizeof(msg)) )
		if (fd_client == -1)
			ec_neg1( fd_client = open(fifo_name, O_RDONLY) )
#ifdef WANT_BUGFIX
		if (fd_client_w == -1)
			ec_neg1( fd_client_w = open(fifo_name, O_WRONLY) )
#endif
		ec_neg1( nread = read(fd_client, &msg, sizeof(msg)) )
		//printf("client got %d from read\n", nread);
		if (nread == 0) {
			errno = ENETDOWN;
			EC_FAIL
		}
		printf("client %ld: %s --> %s\n", (long)getpid(),
		  work[i], msg.sm_data);
sleep(i);
	}
	ec_neg1( close(fd_server) )
	ec_neg1( close(fd_client) )
#ifdef WANT_BUGFIX
	ec_neg1( close(fd_client_w) )
#endif
	ec_neg1( unlink(fifo_name) )
	printf("Client %ld done\n", (long)getpid());
	exit(EXIT_SUCCESS);

EC_CLEANUP_BGN
	exit(EXIT_FAILURE);
EC_CLEANUP_END
}
