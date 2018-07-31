/* oobinline.c:
 * 
 * OOB inline receiver :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

extern void bail(char *on_what);
extern int BindAccept(char *addr);

/*
 * SIGURG signal handler :
 */
static void
sigurg(int signo) {

    write(1,"[SIGURG]\n",9);
    signal(SIGURG,sigurg);
}

/*
 * Emulate the IEEE Std 1003.1g
 * standard function sockatmark(3):
 */
static int
Sockatmark(int s) {
    int z;
    int flag;

    z = ioctl(s,SIOCATMARK,&flag);
    if ( z == -1 )
        return -1;
    return flag ? 1 : 0;
}

int
main(int argc,char **argv) {
    int z;               /* Status */
    int s;               /* Socket */
    int oobinline=1; /* OOB inline */
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

    /*
     * Receive the OOB data inline:
     */
    z = setsockopt(s,
        SOL_SOCKET,
        SO_OOBINLINE,
        &oobinline,
        sizeof oobinline);
    if ( z == -1 )
        bail("setsockopt(2)");

    for (;;) {
        printf("\n[%s]\n",
            Sockatmark(s)
                ? "AT MARK"
                : "No Mark");

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
