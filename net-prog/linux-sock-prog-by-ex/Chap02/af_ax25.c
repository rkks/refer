/* af_ax25.c:
 *
 * AX.25 Address Example :
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/ax25.h>

const ax25_address null_ax25_address =
    { { 0x40,0x40,0x40,0x40,0x40,0x40,0x00 } };

int
main(int argc,char **argv,char **envp) {
    int x;              /* Work index */
    int z;              /* Status return code */
    int sck_ax25;               /* Socket */
    struct full_sockaddr_ax25 adr_ax25;
    int len_ax25;               /* length */
    const char ax25_call[]
        = "VE3WWG" /*SSID*/ "3";

    /*
     * Create a AF_X25 socket (with no digis) :
     */
    sck_ax25 = socket(AF_AX25,SOCK_SEQPACKET,0);

    if ( sck_ax25 == -1 ) {
        /* No socket available : */
        fprintf(stderr,
            "%s: socket(AF_AX25,...)\n",
            strerror(errno));
        exit(1);
    }

    /*
     * Form an AF_AX25 Address :
     */
    adr_ax25.fsa_ax25.sax25_family = AF_AX25;
    strcpy(adr_ax25.fsa_ax25.sax25_call.ax25_call,
        ax25_call);
    adr_ax25.fsa_ax25.sax25_ndigis = 1;
    adr_ax25.fsa_digipeater[0] = null_ax25_address;

    /*
     * Call sign + SSID must be shifted
     * up one bit :
     */
    for ( x=0; x<strlen(ax25_call); ++x )
        adr_ax25.fsa_ax25.sax25_call.ax25_call[x]
            <<= 1;
    len_ax25 = sizeof adr_ax25;

    /*
     * Now bind the address to the socket :
     */
    z = bind(sck_ax25,
        (struct sockaddr *)&adr_ax25,
        len_ax25);

    if ( z == -1 ) {
        /* bind failed : */
        fprintf(stderr,
            "%s: bind() for AF_AX25\n",
            strerror(errno));
        exit(1);
    }

    /*
     * Display all of our bound sockets :
     */
    system("netstat -pa 2>/dev/null | "
        "sed -n '/AX.25 sockets/,$p'");
    return 0;
}
