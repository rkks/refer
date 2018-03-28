#include "udpserver.h"
#include "util.h"

#include <sys/socket.h>
#include <sys/time.h>

#include <asm/types.h>

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <netinet/in.h>

#define RECEIVE_PORT 2000
#define SEND_PORT 2001
#define BUF_SIZE 1500


int ss= 0; /*Socketdescriptor Receive*/
int cs= 0; /*Socketdescriptor Send*/
long total_packets = 0;

int main(void) {
	struct sockaddr_in host_address, target_host_address;	
	int hst_addr_size = sizeof(host_address);
	unsigned char* ip_address;
	unsigned char* target_ip_address;
	char buffer[BUF_SIZE];
	int length = 0;
	int send_result = 0;


	ip_address = (unsigned char*)&host_address.sin_addr.s_addr;

	target_host_address.sin_family=PF_INET;
	target_host_address.sin_port=htons(RECEIVE_PORT);
	target_ip_address = (unsigned char*)&target_host_address.sin_addr.s_addr;
		
	printf("Server started, entering initialiation phase...\n");

	/*open Socket for receiving*/
	ss = create_udp_socket(RECEIVE_PORT); 
	if (ss == -1) {
		perror("socket():");
	        exit(1);
	}
	printf("Successfully opened socket for receiving: %i\n", ss);
	/*open Socket for sending*/
        cs = create_udp_socket(SEND_PORT); 
	if (cs == -1) {
		perror("socket():");
		exit(1);
	}
	printf("Successfully opened socket for sending: %i\n", cs);
	
	/*establish signal handler*/
	signal(SIGINT, sigint);
	printf("Successfully established signal handler for SIGINT\n");
		
	printf("We are in production state, waiting for incoming packets....\n");

	while (1) {
		/*Wait for incoming packet...*/	
		length = recvfrom(ss, buffer, BUF_SIZE, 0, (struct sockaddr*)&host_address, &hst_addr_size);
		if (length < 0) {
			perror("recvfrom():");
			exit(1);
		}

	 	/*printf("Laenge des Packets: %d\n", length );
		printf("Received from: %d.%d.%d.%d\n",ip_address[0],ip_address[1],ip_address[2],ip_address[3]);
		*/

		/*send the packet back...*/
		target_ip_address[0] = ip_address[0];
		target_ip_address[1] = ip_address[1];
		target_ip_address[2] = ip_address[2];
		target_ip_address[3] = ip_address[3];

		send_result = sendto(cs, buffer, length, 0, (struct sockaddr*)&target_host_address, sizeof(target_host_address));
		if (send_result < 0) {
			perror("semdto():");
			exit(1);
		}
		total_packets++;
	}
}

void sigint(int signum) {
	/*Clean up.......*/

	close(ss);
	close(cs);
	
	printf("Server terminating....\n");

	printf("Totally received: %d packets\n", total_packets);
	exit(0);
}
