/* Goal: send large amount of data over TCP to a remote host, and have everything arrive. While doing so, study
   the role of various system calls, ioctrls and socket options.

   Run as 'program-a 1.2.3.4' to connect to 1.2.3.4 port 9876

   Bert Hubert, Netherlabs Computer Consulting BV - bert.hubert@netherlabs.nl
*/

#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>

#ifdef __linux__
#include <linux/sockios.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/* in the interest of brevity, this code does not contain a lot of error checking */

void makeSocketLinger(int fd)
{
        struct linger ling;
        ling.l_onoff=1;
        ling.l_linger=30;
        setsockopt(fd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
}

void depleteSendBuffer(int fd) 
{
#ifdef __linux__
	int lastOutstanding=-1;
	for(;;) {
		int outstanding;
		ioctl(fd, SIOCOUTQ, &outstanding);
		if(outstanding != lastOutstanding) 
			printf("Outstanding: %d\n", outstanding);
		lastOutstanding = outstanding;
		if(!outstanding)
			break;
		usleep(1000);
	}
#endif
}


int main(int argc, char** argv)
{
	signal(SIGPIPE, SIG_IGN);
	int sock=socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in remote;
	remote.sin_family=AF_INET;
	inet_pton(AF_INET, argc > 1 ? argv[1] : "127.0.0.1", &remote.sin_addr);
	remote.sin_port = ntohs(9876);
	
	makeSocketLinger(sock);   
	
	if(connect(sock, (struct sockaddr*)&remote, sizeof(remote)) < 0) {
		perror("Connecting to remote");
		exit(1);
	}
	
	char buffer[4000];
	int n, res;

	for(n=0; n < 250 ; ++n) {
		res=write(sock, buffer, sizeof(buffer));
		if(res != sizeof(buffer)) {
			if(res < 0)
				perror("writing");
			else
				fprintf(stderr,"Partial write\n");
			exit(1);
		}
	}
	
	shutdown(sock, SHUT_WR);   /* inform remote that we are done */
	
	depleteSendBuffer(sock);
	
	// now wait for the remote to close its socket
	
	for(;;) {
		res=read(sock, buffer, sizeof(buffer));
		if(res < 0) {
			perror("reading");
			exit(1);
		}
		if(!res) {
			printf("Correct EOF\n");
			break;
		}
	}
	close(sock);
	return 0;
}
