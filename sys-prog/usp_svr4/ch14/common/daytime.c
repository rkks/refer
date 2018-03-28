#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

#define SERVICENAME "daytime"

int
main(int argc, char **argv)
{
    int n, s, len;
    char buf[1024];
    char *hostname;
    struct hostent *hp;
    struct servent *sp;
    struct sockaddr_in name, from;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s hostname [hostname...]\n", *argv);
        exit(1);
    }

    /*
     * Look up our service.  We want the UDP version.
     */
    if ((sp = getservbyname(SERVICENAME, "udp")) == NULL) {
        fprintf(stderr, "%s/udp: unknown service.\n", SERVICENAME);
        exit(1);
    }

    while (--argc) {
        hostname = *++argv;

        /*
         * Look up the host's network address.
         */
        if ((hp = gethostbyname(hostname)) == NULL) {
            fprintf(stderr, "%s: unknown host.\n", hostname);
            continue;
        }

        /*
         * Create a socket in the INET
         * domain.
         */
        if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("socket");
            exit(1);
        }

        /*
         * Create the address of the server.
         */
        memset(&name, 0, sizeof(struct sockaddr_in));

        name.sin_family = AF_INET;
        name.sin_port = sp->s_port;
        memcpy(&name.sin_addr, hp->h_addr_list[0], hp->h_length);
        len = sizeof(struct sockaddr_in);

        /*
         * Send a packet to the server.
         */
        memset(buf, 0, sizeof(buf));

        n = sendto(s, buf, sizeof(buf), 0, (struct sockaddr *) &name,
                   sizeof(struct sockaddr_in));

        if (n < 0) {
            perror("sendto");
            exit(1);
        }

        /*
         * Receive a packet back.
         */
        len = sizeof(struct sockaddr_in);
        n = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &from, &len);

        if (n < 0) {
            perror("recvfrom");
            exit(1);
        }

        /*
         * Print the packet.
         */
        buf[n] = '\0';
        printf("%s: %s", hostname, buf);

        /*
         * Close the socket.
         */
        close(s);
    }

    exit(0);
}
