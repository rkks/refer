#include "udpclient.h"
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

#include<netinet/in.h>

#define NUMBER_OF_MESUREMENTS_PER_AMOUNT_OF_DATA 100000 /*how often to measure travelling time with one certain amount of data*/

#define RECEIVE_PORT 2000
#define SEND_PORT 2001
#define BUF_SIZE 1500


int ss = 0; /*Socketdescriptor*/
int cs = 0;
void* buffer = NULL;
long total_sent_packets = 0;

int main(void) {
 	char buffer[BUF_SIZE];
	unsigned char dest_ip[4];
	struct sockaddr_in target_host_address;
	unsigned char* target_address_holder;
	
	/*stuff for time measuring: */
	struct timeval begin;
        struct timeval end;
        struct timeval result;
        unsigned long long allovertime;


	int i,j,k, sent, length;

	/*Init destination IP address*/
	/*sorry for me hardcoding it, too lazy to do make it dynamic.....*/
	dest_ip[0] = 10;
	dest_ip[1] = 0;
	dest_ip[2] = 0;
	dest_ip[3] = 12;

	printf("Client started, entering initialiation phase...\n");

	/*open sockets*/
	ss = create_udp_socket(RECEIVE_PORT);
	if (ss == -1) {
		perror("socket():");
	        exit(1);
	}
	printf("Successfully opened socket for receiving: %i\n", ss);

	cs = create_udp_socket(SEND_PORT);
	if (cs == -1) {
		perror("socket():");
		exit(1);
	}
	printf("Successfully opened socket for sending: %i\n", cs);
			
	
	
	/*establish signal handler*/
	signal(SIGINT, sigint);
	printf("Successfully established signal handler for SIGINT\n");

	/*init random number generator*/
	srand(time(NULL));

	/*init target address structure*/
	target_host_address.sin_family=PF_INET;
	target_host_address.sin_port=htons(RECEIVE_PORT);
	target_address_holder=(unsigned char*)&target_host_address.sin_addr.s_addr;
	target_address_holder[0]=dest_ip[0];
	target_address_holder[1]=dest_ip[1];
	target_address_holder[2]=dest_ip[2];
	target_address_holder[3]=dest_ip[3];
							      
	printf("We are in production state, sending packets....\n");

	for (i = 50; i <= 1500; i += 50) {

		allovertime = 0;
		
		for (k = 0; k < NUMBER_OF_MESUREMENTS_PER_AMOUNT_OF_DATA; k++) {
			/*fill it with random data....*/
			for (j = 0; j < BUF_SIZE; j++) {
				buffer[j] = (unsigned char)((int) (255.0*rand()/(RAND_MAX+1.0)));
			}
	
			/*clear the timers:*/
			timerclear(&begin);
		        timerclear(&end);
	
			/*get time before sending.....*/
		        gettimeofday(&begin,NULL);
					
	
			/*send packet*/
			sent = sendto(cs, buffer, i, 0, (struct sockaddr*)&target_host_address, sizeof(struct sockaddr));
			if (sent == -1) {
				perror("sendto():");
				exit(1);
			}
				
			/*Wait for incoming packet...*/
			length = recvfrom(ss, buffer, BUF_SIZE, 0, NULL, NULL);
			if (length == -1) {
				perror("recvfrom():");
				exit(1);
			}
	
			/*get time after sending.....*/
			gettimeofday(&end,NULL);
			/*...and calculate difference.........*/
			timersub(&end,&begin,&result); 
	
		        allovertime += ((result.tv_sec * 1000000 ) + result.tv_usec );
	
			total_sent_packets++;
		}

		printf("Sending %i bytes takes %lld microseconds in average\n",i ,allovertime/NUMBER_OF_MESUREMENTS_PER_AMOUNT_OF_DATA);
	}
}

void sigint(int signum) {
	/*Clean up.......*/

	close(ss);
	close(cs);

	printf("Client terminating....\n");

	printf("Totally sent: %d packets\n", total_sent_packets);
	exit(0);
}
