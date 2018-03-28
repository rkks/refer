#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netconfig.h>
#include <tiuser.h>
#include <netdir.h>
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
    struct netconfig *ncp;
    struct nd_hostserv ndh;
    struct nd_addrlist *nal;
    struct t_bind *reqp, *retp;
    char buf[1024], hostname[64];

    /*
     * Get our local host name.
     */
    if (gethostname(hostname, sizeof(hostname)) < 0) {
        perror("gethostname");
        exit(1);
    }

    /*
     * Select the TCP transport provider.
     */
    if ((ncp = getnetconfigent("tcp")) == NULL) {
        nc_perror("tcp");
        exit(1);
    }

    /*
     * Get a host and service address for our host.  Since our
     * port number is not registered in the services file, we
     * send down the ASCII string representation of it.
     */
    sprintf(buf, "%d", PORTNUMBER);
    ndh.h_host = hostname;
    ndh.h_serv = buf;

    if (netdir_getbyname(ncp, &ndh, &nal) != 0) {
        netdir_perror(hostname);
        exit(1);
    }

    /*
     * Create a transport endpoint.
     */
     if ((fd = t_open(ncp->nc_device, O_RDWR, NULL)) < 0) {
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
    
    memcpy(&reqp->addr, &nal->n_addrs[0], sizeof(struct netbuf));
    reqp->qlen = 5;

    if (t_bind(fd, reqp, retp) < 0) {
        t_error("t_bind");
        exit(1);
    }

    if (retp->addr.len != nal->n_addrs[0].len ||
        memcmp(retp->addr.buf, nal->n_addrs[0].buf, retp->addr.len) != 0) {
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
