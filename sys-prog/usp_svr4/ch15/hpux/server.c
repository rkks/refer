#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <tiuser.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#define PORTNUMBER  12345

extern int t_errno;

int
main(void)
{
    int n, fd, flags;
    struct t_call *callp;
    struct t_bind *reqp, *retp;
    struct sockaddr_in loc_addr;
    char buf[1024], hostname[64];

    /*
     * Get our local host name.
     */
    if (gethostname(hostname, sizeof(hostname)) < 0) {
        perror("gethostname");
        exit(1);
    }

    /*
     * Create a host and service address for our host.
     */
    memset((char *) &loc_addr, 0, sizeof(struct sockaddr_in));
    loc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    loc_addr.sin_port = htons(PORTNUMBER);
    loc_addr.sin_family = AF_INET;

    /*
     * Create a transport endpoint.
     */
     if ((fd = t_open("/dev/inet_cots", O_RDWR, NULL)) < 0) {
        t_error("t_open");
        exit(1);
     }

    /*
     * Bind the address to the transport endpoint.
     */
    retp = (struct t_bind *) t_alloc(fd, T_BIND, T_ADDR);
    reqp = (struct t_bind *) t_alloc(fd, T_BIND, T_ADDR);

    if (reqp == NULL || retp == NULL) {
        t_error("t_alloc");
        exit(1);
    }
    
    reqp->addr.maxlen = sizeof(struct sockaddr_in);
    reqp->addr.len = sizeof(struct sockaddr_in);
    reqp->addr.buf = (char *) &loc_addr;
    reqp->qlen = 5;

    if (t_bind(fd, reqp, retp) < 0) {
        t_error("t_bind");
        exit(1);
    }

    if (retp->addr.len != reqp->addr.len ||
        memcmp(retp->addr.buf, reqp->addr.buf, retp->addr.len) != 0) {
        fprintf(stderr, "did not bind requested address.\n");
        exit(1);
    }

    /*
     * Allocate a call structure.
     */
    callp = (struct t_call *) t_alloc(fd, T_CALL, T_ALL);

    if (callp == NULL) {
        t_error("t_alloc");
        exit(1);
    }

    /*
     * Listen for a connection.
     */
    if (t_listen(fd, callp) < 0) {
        t_error("t_listen");
        exit(1);
    }

    /*
     * Accept a connect on the same file descriptor used for listening.
     */
    if (t_accept(fd, fd, callp) < 0) {
        t_error("t_accept");
        exit(1);
    }

    /*
     * Read from the network until end-of-file and
     * print what we get on the standard output.
     */
    while ((n = t_rcv(fd, buf, sizeof(buf), &flags)) > 0)
        write(1, buf, n);

    /*
     * Release the connection.
     */
    t_rcvrel(fd);
    t_sndrel(fd);

    t_unbind(fd);
    t_close(fd);
    exit(0);
}
