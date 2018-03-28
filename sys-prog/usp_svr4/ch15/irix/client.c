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
    int n, fd;
    struct t_call *callp;
    struct netconfig *ncp;
    struct nd_hostserv ndh;
    struct nd_addrlist *nal;
    char buf[32], hostname[64];

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
    callp = (struct t_call *) t_alloc(fd, T_CALL, 0);

    if (callp == NULL) {
        t_error("t_alloc");
        exit(1);
    }

    /*
     * Construct the connection request.
     */
    memcpy(&callp->addr, &nal->n_addrs[0], sizeof(struct netbuf));

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
