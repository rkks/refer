#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <tiuser.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>

#define PORTNUMBER  12345

extern int t_errno;

int
main(void)
{
    int n, fd;
    struct hostent *hp;
    struct t_call *callp;
    char buf[32], hostname[64];
    struct sockaddr_in rem_addr;

    /*
     * Get our local host name.
     */
    if (gethostname(hostname, sizeof(hostname)) < 0) {
        perror("gethostname");
        exit(1);
    }

    /*
     * Get the address of our host.
     */
    if ((hp = gethostbyname(hostname)) == NULL) {
        fprintf(stderr, "Cannot find address for %s\n", hostname);
        exit(1);
    }

    /*
     * Create a host and service address for our host.
     */
    memset((char *) &rem_addr, 0, sizeof(struct sockaddr_in));
    memcpy((char *) &rem_addr.sin_addr.s_addr, (char *) hp->h_addr,
           hp->h_length);
    rem_addr.sin_port = htons(PORTNUMBER);
    rem_addr.sin_family = AF_INET;

    /*
     * Create a transport endpoint.
     */
     if ((fd = t_open("/dev/inet_cots", O_RDWR, NULL)) < 0) {
        t_error("t_open");
        exit(1);
     }

     /*
      * Bind an arbitrary address to the transport
      * endpoint.
      */
     if (t_bind(fd, NULL, NULL) < 0) {
        t_error("t_bind");
        exit(1);
     }

    /*
     * Allocate a connection structure.
     */
    callp = (struct t_call *) t_alloc(fd, T_CALL, T_ADDR);

    if (callp == NULL) {
        t_error("t_alloc");
        exit(1);
    }

    /*
     * Construct the connection request.
     */
    callp->addr.maxlen = sizeof(struct sockaddr_in);
    callp->addr.len = sizeof(struct sockaddr_in);
    callp->addr.buf = (char *) &rem_addr;
    callp->udata.len = 0;
    callp->opt.len = 0;

    /*
     * Connect to the server.
     */
    if (t_connect(fd, callp, NULL) < 0) {
        if (t_errno == TLOOK) {
            if (t_rcvdis(fd, NULL) < 0) {
                t_error("t_rcvdis");
                exit(1);
            }
        }
        else {
            t_error("t_connect");
            exit(1);
        }
    }
        

    /*
     * Read from standard input, and copy the
     * data to the network.
     */
    while ((n = read(0, buf, sizeof(buf))) > 0) {
        if (t_snd(fd, buf, n, 0) < 0) {
            t_error("t_snd");
            exit(1);
        }
    }

    /*
     * Release the connection.
     */
    t_sndrel(fd);
    t_rcvrel(fd);

    t_unbind(fd);
    t_close(fd);
    exit(0);
}
