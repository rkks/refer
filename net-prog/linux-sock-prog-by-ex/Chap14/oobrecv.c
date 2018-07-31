/* oobrecv.c:
 * 
 * Example OOB receiver :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>

extern void bail(char *on_what);
extern int BindAccept(char *addr);

static int s = -1;  /* Socket */

/*
 * SIGURG signal handler :
 */
static void
sigurg(int signo) {
    int n;
    char buf[256];

    n = recv(s,buf,sizeof buf,MSG_OOB);
    if ( n < 0 )
        bail("recv(2)");

    buf[n] = 0;
    printf("URG '%s' (%d)\n",
        buf,n);
      
    signal(SIGURG,sigurg);
}

int
main(int argc,char **argv) {
    int z;            /* Status */
    char buf[256];


    /*
     * Use a server address from the command
     * line, if one has been provided.
     * Otherwise, this program will default
     * to using the arbitrary address
     * 127.0.0.1 :
     */
    s = BindAccept(argc >= 2
        ? argv[1]
        : "127.0.0.1:9011");

    /*
     * Establish ownership :
     */
    z = fcntl(s,F_SETOWN,getpid());
    if ( z == -1 )
        bail("fcntl(2)");

    /*
     * Catch SIGURG :
     */
    signal(SIGURG,sigurg);

    for (;;) {
        z = recv(s,buf,sizeof buf,0);
        if ( z == -1 )
            bail("recv(2)");
        if ( z == 0 )
            break;
        buf[z] = 0;

        printf("rcv '%s' (%d)\n",
            buf, z);
    }

    close(s);
    return 0;
}
