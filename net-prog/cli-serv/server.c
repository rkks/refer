#include "server.h"
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

int s = 0; /*Socketdescriptor*/
void* buffer = NULL;
long total_packets = 0;
long answered_packets = 0;

int main(void) {
	buffer = (void*)malloc(BUF_SIZE); 	/*Buffer for Ethernet Frame*/
	unsigned char* etherhead = buffer;	/*Pointer to Ethenet Header*/
	struct ethhdr *eh = (struct ethhdr *)etherhead; /*Another pointer to ethernet header*/

	unsigned char src_mac[6];		/*our MAC address*/

	struct ifreq ifr;
	struct sockaddr_ll socket_address;
	int ifindex = 0;			/*Ethernet Interface index*/
	int i;
	int length;				/*length of received packet*/
	int sent;

	printf("Server started, entering initialiation phase...\n");

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
	socket_address.sll_addr[6]  = 0x00; 
	socket_address.sll_addr[7]  = 0x00;
														
	
	/*establish signal handler*/
	signal(SIGINT, sigint);
	printf("Successfully established signal handler for SIGINT\n");
		
	printf("We are in production state, waiting for incoming packets....\n");

	while (1) {
		/*Wait for incoming packet...*/	
		length = recvfrom(s, buffer, BUF_SIZE, 0, NULL, NULL);
		if (length == -1) {
			perror("recvfrom():");
			exit(1);
		}

		/*See if we should answer (Ethertype == 0x0 && destination address == our MAC)*/
		if (eh->h_proto == ETH_P_NULL && memcmp( (const void*)eh->h_dest, (const void*)src_mac, ETH_MAC_LEN) == 0 ) {
		
			/*exchange addresses in buffer*/
			memcpy( (void*)etherhead, (const void*)(etherhead+ETH_MAC_LEN), ETH_MAC_LEN);
			memcpy( (void*)(etherhead+ETH_MAC_LEN), (const void*)src_mac, ETH_MAC_LEN);
			
			/*prepare sockaddr_ll*/
			socket_address.sll_addr[0]  = eh->h_dest[0];
			socket_address.sll_addr[1]  = eh->h_dest[1];
			socket_address.sll_addr[2]  = eh->h_dest[2];
			socket_address.sll_addr[3]  = eh->h_dest[3];
			socket_address.sll_addr[4]  = eh->h_dest[4];
			socket_address.sll_addr[5]  = eh->h_dest[5];

			/*send answer*/
			sent = sendto(s, buffer, length-4, 0, (struct sockaddr*)&socket_address, sizeof(socket_address));
			if (sent == -1) {
				perror("sendto():");
				exit(1);
			}
			
			answered_packets++;
		}

		total_packets++;
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
	
	printf("Server terminating....\n");

	printf("Totally received: %d packets\n", total_packets);
	printf("Answered %d packets\n", answered_packets);
	exit(0);
}
