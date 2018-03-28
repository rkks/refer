#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <tiuser.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>

#define SERVICENAME "daytime"

extern int t_errno;

int
main(int argc, char **argv)
{
    int fd, flags;
    struct hostent *hp;
    struct servent *sp;
    struct t_unitdata *udp;
    struct nd_addrlist *nal;
    struct sockaddr_in rem_addr;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s hostname [hostname...]\n", *argv);
        exit(1);
    }

    if ((sp = getservbyname(SERVICENAME, "udp")) == NULL) {
        fprintf(stderr, "%s/udp: unknown service\n", SERVICENAME);
        exit(1);
    }

    while (--argc) {
        if ((hp = gethostbyname(*++argv)) == NULL) {
            fprintf(stderr, "%s: unknown host\n", *argv);
            continue;
        }

        /*
         * Create a transport endpoint.
         */
        if ((fd = t_open("/dev/inet_clts", O_RDWR, NULL)) < 0) {
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
         * Create a host and service address for our host.
         */
        memset((char *) &rem_addr, 0, sizeof(struct sockaddr_in));
        memcpy((char *) &rem_addr.sin_addr.s_addr, (char *) hp->h_addr,
               hp->h_length);
        rem_addr.sin_port = sp->s_port;
        rem_addr.sin_family = AF_INET;

        /*
         * Construct the datagram.
         */
        udp->addr.maxlen = sizeof(struct sockaddr_in);
        udp->addr.len = sizeof(struct sockaddr_in);
        udp->addr.buf = (char *) &rem_addr;
        udp->opt.buf = (char *) 0;
        udp->opt.maxlen = 0;
        udp->opt.len = 0;
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
