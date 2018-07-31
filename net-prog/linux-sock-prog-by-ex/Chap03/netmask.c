/* netmask.c:
 * 
 * Classify an IP address:
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int
main(int argc,char **argv) {
    int x;               /* Index variable */
    struct sockaddr_in adr_inet;/* AF_INET */
    int len_inet;               /* length  */
    unsigned msb; /* Most significant byte */
    char class;
    char *netmask;
    static struct {
        unsigned char   ip[4];
    } addresses[] = {
        { { 44,135,86,12 } },
        { { 127,0,0,1 } },
        { { 172,16,23,95 } },
        { { 192,168,9,1 } }
    };

    for ( x=0; x<4; ++x ) {
        /*
         * Set up the socket address, to
         * demonstrate how to classify it:
         */
        memset(&adr_inet,0,sizeof adr_inet);
        adr_inet.sin_family = AF_INET;
        adr_inet.sin_port = htons(9000);
        memcpy(&adr_inet.sin_addr.s_addr,
            addresses[x].ip,4);
        len_inet = sizeof adr_inet;

        /*
         * Classify this address:
         *
         * 1. Get the Most Significant Byte
         * 2. Classify by that byte
         */
        msb = *(unsigned char *)
            &adr_inet.sin_addr.s_addr;

        if ( (msb & 0x80) == 0x00 ) {
            class = 'A';
            netmask = "255.0.0.0";
        } else if ( (msb & 0xC0) == 0x80 ) {
            class = 'B';
            netmask = "255.255.0.0";
        } else if ( (msb & 0xE0) == 0xC0 ) {
            class = 'C';
            netmask = "255.255.255.0";
        } else if ( (msb & 0xF0) == 0xE0 ) {
            class = 'D';
            netmask = "255.255.255.255";
        } else  {
            class = 'E';
            netmask = "255.255.255.255";
        }

        printf("Address %u.%u.%u.%u is class %c "
            "netmask %s\n",
            addresses[x].ip[0],
            addresses[x].ip[1],
            addresses[x].ip[2],
            addresses[x].ip[3],
            class,
            netmask);
    }

    return 0;
}
