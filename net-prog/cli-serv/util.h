#include "ethertest.h"

void print_eth_mac_addr(unsigned char* eth_header); /*prints both MAC adresses from Ethernet Header*/
void hexdump(unsigned char *buf, int nbytes);	    /*hexdumps some memory to stdout*/	
int create_udp_socket(int port);		    /*returns a socket despriptor the a newly created UDP Socket*/	
