/* af_x25.c:
 *
 * X.25 Socket Address Example :
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/x25.h>

/*
 * This function reports the error and
 * exits back to the shell :
 */
static void
bail(const char *on_what) {
    perror(on_what);
    exit(1);
}

int
main(int argc,char **argv,char **envp) {
    int z;          /* Status return code */
    int sck_x25;                /* Socket */
    struct sockaddr_x25 adr_x25;/* AF_X25 */
    int len_x25;                /* length */
    const char x25_host[]   /* X.121 addr */
        = "79400900";

    /* Create a AF_X25 socket */
    sck_x25 = socket(AF_X25,SOCK_SEQPACKET,0);

    if ( sck_x25 == -1 )
        bail("socket()");

    /* Form an AF_X25 Address */
    adr_x25.sx25_family = AF_X25;
    strcpy(adr_x25.sx25_addr.x25_addr,x25_host);
    len_x25 = sizeof adr_x25;

    /* Bind the address to the socket */
    z = bind(sck_x25,
        (struct sockaddr *)&adr_x25,
        len_x25);

    if ( z == -1 )
        bail("bind()");

    puts("X.25 SOCKETS :");
    system("cat /proc/net/x25");
    return 0;
}
