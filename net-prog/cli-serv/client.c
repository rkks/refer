#include "client.h"
#include "util.h"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <asm/types.h>

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

#define BUF_SIZE ETH_FRAME_TOTALLEN
#define NUMBER_OF_MESUREMENTS_PER_AMOUNT_OF_DATA 100000 /*how often to measure travelling time with one certain amount of data*/

int s = 0; /*Socketdescriptor*/
void* buffer = NULL;
long total_sent_packets = 0;

int main(void) {
	buffer = (void*)malloc(BUF_SIZE); 	/*Buffer for ethernet frame*/
	unsigned char* etherhead = buffer;	/*Pointer to ethenet header*/
	unsigned char* data = buffer + 14;	/*Userdata in ethernet frame*/
	struct ethhdr *eh = (struct ethhdr *)etherhead; /*Another pointer to ethernet header*/

	unsigned char src_mac[6];		/*our MAC address*/
	unsigned char dest_mac[6] = {0x00, 0x04, 0x75, 0xC8, 0x28, 0xE5};	/*other host MAC address, hardcoded...... :-(*/

	struct ifreq ifr;
	struct sockaddr_ll socket_address;
	int ifindex = 0;			/*Ethernet Interface index*/
	int i,j,k;
	int length;				/*length of received packet*/
	int sent;				/*length of sent packet*/

	/*stuff for time measuring: */
	struct timeval begin;
        struct timeval end;
        struct timeval result;
        unsigned long long allovertime;
				

	printf("Client started, entering initialiation phase...\n");

	/*open socket*/
	s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (s == -1) {
		perror("socket():");
	        exit(1);
	}
	printf("Successfully opened socket: %i\n", s);
	
	/*retrieve ethernet interface index*/
	strncpy(ifr.ifr_name, DEVICE, IFNAMSIZ);
	if (ioctl(s, SIOCGIFINDEX, &ifr) == -1) {
		perror("SIOCGIFINDEX");
		exit(1);
	}
	ifindex = ifr.ifr_ifindex;
	printf("Successfully got interface index: %i\n", ifindex);
	
	/*retrieve corresponding MAC*/
	if (ioctl(s, SIOCGIFHWADDR, &ifr) == -1) {
		perror("SIOCGIFINDEX");
		exit(1);
	}
        for (i = 0; i < 6; i++) {
		src_mac[i] = ifr.ifr_hwaddr.sa_data[i];
	}
	printf("Successfully got our MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n", 
			src_mac[0],src_mac[1],src_mac[2],src_mac[3],src_mac[4],src_mac[5]);

	/*prepare sockaddr_ll*/
	socket_address.sll_family   = PF_PACKET;
	socket_address.sll_protocol = htons(ETH_P_IP);
	socket_address.sll_ifindex  = ifindex;
	socket_address.sll_hatype   = ARPHRD_ETHER;
	socket_address.sll_pkttype  = PACKET_OTHERHOST;
	socket_address.sll_halen    = ETH_ALEN;
	socket_address.sll_addr[0]  = dest_mac[0];
	socket_address.sll_addr[1]  = dest_mac[1];
        socket_address.sll_addr[2]  = dest_mac[2];
        socket_address.sll_addr[3]  = dest_mac[3];
        socket_address.sll_addr[4]  = dest_mac[4];
        socket_address.sll_addr[5]  = dest_mac[5];
	socket_address.sll_addr[6]  = 0x00; 
	socket_address.sll_addr[7]  = 0x00;
														
	
	/*establish signal handler*/
	signal(SIGINT, sigint);
	printf("Successfully established signal handler for SIGINT\n");

	/*init random number generator*/
	srand(time(NULL));
		
	printf("We are in production state, sending packets....\n");

	for (i = 50; i <= 1500; i += 50) {

		allovertime = 0;
		
		for (k = 0; k < NUMBER_OF_MESUREMENTS_PER_AMOUNT_OF_DATA; k++) {
			/*prepare buffer*/
			memcpy((void*)buffer, (void*)dest_mac, ETH_MAC_LEN);
			memcpy((void*)(buffer+ETH_MAC_LEN), (void*)src_mac, ETH_MAC_LEN);
			eh->h_proto = ETH_P_NULL;
			/*fill it with random data....*/
			for (j = 0; j < i; j++) {
				data[j] = (unsigned char)((int) (255.0*rand()/(RAND_MAX+1.0)));
			}
	
			/*clear the timers:*/
			timerclear(&begin);
		        timerclear(&end);
	
			/*get time before sending.....*/
		        gettimeofday(&begin,NULL);
					
	
			/*send packet*/
			sent = sendto(s, buffer, i+ETH_HEADER_LEN, 0, (struct sockaddr*)&socket_address, sizeof(socket_address));
			if (sent == -1) {
				perror("sendto():");
				exit(1);
			}
				
			/*Wait for incoming packet...*/
			length = recvfrom(s, buffer, BUF_SIZE, 0, NULL, NULL);
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

	struct ifreq ifr;

        if (s == -1)
        	return;
	
	strncpy(ifr.ifr_name, DEVICE, IFNAMSIZ);
	ioctl(s, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags &= ~IFF_PROMISC;
	ioctl(s, SIOCSIFFLAGS, &ifr);
	close(s);

	free(buffer);
	
	printf("Client terminating....\n");

	printf("Totally sent: %d packets\n", total_sent_packets);
	exit(0);
}
