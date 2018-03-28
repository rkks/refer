#include "util.h"

#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>

void print_eth_mac_addr(unsigned char* eth_header) {
        int i;
        struct ethhdr *eh;

        eh = (struct ethhdr *)eth_header;

        for (i = 0; i < 5; ++i) {
		printf("%02X:", (int)eh->h_source[i]);
	}
	printf("%02X -> ", (int)eh->h_source[i]);
		
	for (i = 0; i < 5; ++i) {
		printf("%02X:", (int)eh->h_dest[i]);
	}
	printf("%02X", (int)eh->h_dest[i]);
	printf("\n");
								
}

void hexdump(unsigned char *buf, int nbytes) {
	int i, j;

	for(i = 0; i < nbytes; i += 16) {
        	printf("%04X : ", i);
		for (j = 0; j < 16 && i + j < nbytes; j++)
			printf("%2.2X ", buf[i + j]);
		for (; j < 16; j++) {
			 printf("   ");
		}
		printf(": ");
		for (j = 0; j < 16 && i + j < nbytes; j++) {
			char c = toascii(buf[i + j]);
			printf("%c", isalnum(c) ? c : '.');
		}
		printf("\n");
	}
}

int create_udp_socket(int port) {
	int    s;
	struct sockaddr_in     host_address;
	s=socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s < 0) {
		perror("socket()");
		return -1;
	}
	memset((void*)&host_address, 0, sizeof(host_address));
	host_address.sin_family=AF_INET;
	host_address.sin_addr.s_addr=INADDR_ANY;
	host_address.sin_port=htons(port);
	if (bind(s, (struct sockaddr*)&host_address, sizeof(host_address)) < 0) {
		perror("bind()");
		return -1;
	}
	return s;
}


