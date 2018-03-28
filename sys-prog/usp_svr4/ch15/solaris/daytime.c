#include <netconfig.h>
#include <netdir.h>
#include <tiuser.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

#define SERVICENAME "daytime"

extern int t_errno;

int
main(int argc, char **argv)
{
    int fd, flags;
    struct netconfig *ncp;
    struct nd_hostserv ndh;
    struct t_unitdata *udp;
    struct nd_addrlist *nal;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s hostname [hostname...]\n", *argv);
        exit(1);
    }

    /*
     * Select the UDP transport provider.
     */
    if ((ncp = getnetconfigent("udp")) == NULL) {
        nc_perror("udp");
        exit(1);
    }

    while (--argc) {
        ndh.h_host = *++argv;
        ndh.h_serv = SERVICENAME;

        /*
         * Get a host and service address for this host.
         */
        if (netdir_getbyname(ncp, &ndh, &nal) != 0) {
            netdir_perror(*argv);
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
         * Allocate a datagram.
         */
        udp = (struct t_unitdata *) t_alloc(fd, T_UNITDATA, T_ALL);

        if (udp == NULL) {
            t_error("t_alloc");
            exit(1);
        }

        /*
         * Construct the datagram.
         */
        memcpy(&udp->addr, &nal->n_addrs[0], sizeof(struct netbuf));
        udp->udata.len = 1;

        /*
         * Send a packet to the server.
         */
        if (t_sndudata(fd, udp) < 0) {
            t_error("t_sndudata");
            exit(1);
        }

        /*
         * Receive a packet back.
         */
        if (t_rcvudata(fd, udp, &flags) < 0) {
            if (t_errno == TLOOK) {
                if (t_rcvuderr(fd, NULL) < 0) {
                    t_error("t_rcvuderr");
                    exit(1);
                }
            }
            else {
                t_error("t_rcvudata");
                exit(1);
            }
        }

        /*
         * Print the packet.
         */
        udp->udata.buf[udp->udata.len] = '\0';
        printf("%s: %s", *argv, udp->udata.buf);

        /*
         * Shut down the connection.
         */
        t_unbind(fd);
        t_close(fd);
    }

    exit(0);
}
