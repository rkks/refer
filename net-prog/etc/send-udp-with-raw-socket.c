/* Sends a UDP packet with raw sockets, to demonstrate their use. 

A big warning: with IPv6, raw sockets typically do not work (see RFC
3542 and
http://lists.freebsd.org/pipermail/freebsd-pf/2006-May/002174.html for
explanation and discussion). The code works on Linux but this is pure
luck. On FreeBSD and NetBSD, it sends the wrong packet, because some
fields are rewritten.

*/

#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#define __FAVOR_BSD
#include <netinet/udp.h>

#define MAXHOSTNAMELEN 256
#define MSGSIZE 64
#define PORT 42
#define TTL 23
#define MESSAGE "azertyuiopqsdf"

/* No constant for UDP on NetBSD */
#ifndef SOL_UDP
#define SOL_UDP 17
#endif

static int      verbose_flag, v4_flag, v6_flag = 0;
static char     source_addr[INET6_ADDRSTRLEN];
static int      port = 0;

static struct option long_options[] = {
    {"verbose", no_argument, &verbose_flag, 1},
    {"ipv4", no_argument, &v4_flag, 1},
    {"ipv6", no_argument, &v6_flag, 1},
    {"source", required_argument, NULL, 0},
    {"port", required_argument, NULL, 0},
    {0, 0, 0, 0}
};

static char     progname[MAXHOSTNAMELEN + 1];

struct opacket4 {
    struct ip       ip;
    struct udphdr   udp;
    char            payload[MSGSIZE];
} __attribute__ ((packed));

struct opacket6 {
    struct ip6_hdr  ip;
    struct udphdr   udp;
    char            payload[MSGSIZE];
} __attribute__ ((packed));


void
usage()
{
    printf("Usage: %s [-v46] hostname\n", progname);
}

char           *
text_of(struct sockaddr *address)
{
    char           *text = malloc(INET6_ADDRSTRLEN);
    struct sockaddr_in *address_v4;
    struct sockaddr_in6 *address_v6;
    if (address->sa_family == AF_INET) {
        address_v4 = (struct sockaddr_in *) address;
        inet_ntop(AF_INET, &address_v4->sin_addr, text, INET_ADDRSTRLEN);
    } else if (address->sa_family == AF_INET6) {
        address_v6 = (struct sockaddr_in6 *) address;
        inet_ntop(AF_INET6, &address_v6->sin6_addr, text, INET6_ADDRSTRLEN);
    } else {
        strcpy(text, "Unknown address family");
    }
    return text;
}

int
main(int argc, char **argv)
{
    char            c;
    char            hostname[MAXHOSTNAMELEN + 1];
    char            message[MSGSIZE];
    int             status;
    struct addrinfo hints_numeric, hints;
    struct addrinfo *result, *source;
    size_t          packetsize, headersize, extraipheadersize;
    struct opacket4 op4;
    struct opacket6 op6;
    struct sockaddr_in *sockaddr4;
    struct sockaddr_in6 *sockaddr6;
    void           *packet;
    int             on = 1;
    strncpy(progname, argv[0], MAXHOSTNAMELEN);
    progname[MAXHOSTNAMELEN] = 0;
    int             sd;         /* Socket Descriptor */
    ssize_t         num;
    strcpy(message, MESSAGE);
    strcpy(source_addr, "");
    while (true) {
        /* getopt_long stores the option index here.  */
        int             option_index = 0;

        c = getopt_long(argc, argv, "v46s:p:", long_options, &option_index);

        /* Detect the end of the options.  */
        if (c == -1)
            break;

        switch (c) {
        case 0:
            /* If this option set a flag, do nothing else now.  */
            if (long_options[option_index].flag != 0)
                break;
            if (strcmp(long_options[option_index].name, "source") == 0) {
                strcpy(source_addr, optarg);
            } else if (strcmp(long_options[option_index].name, "port") == 0) {
                port = strtol(optarg, NULL, 10);        /* Assume the port number is 
                                                         * in base 10 */
                if (port == 0) {
                    fprintf(stderr, "Invalid port number %s\n", optarg);
                    abort();
                }
            } else {
                fprintf(stderr, "Internal error, option %s not known\n",
                        long_options[option_index].name);
            }
            break;

        case 'v':
            verbose_flag = 1;
            break;
        case '4':
            v4_flag = 1;
            break;
        case '6':
            v6_flag = 1;
            break;

        case 's':
            strcpy(source_addr, optarg);
            break;

        case 'p':
            port = strtol(optarg, NULL, 10);    /* Assume the port number is in base 
                                                 * 10 */
            if (port == 0) {
                fprintf(stderr, "Invalid port number %s\n", optarg);
                abort();
            }
            break;

        case '?':
            usage();
            break;

        default:
            abort();
        }
    }

    /* Print any remaining command line arguments (not options).  */
    if (argc - optind != 1) {
        usage();
        abort();
    }
    strncpy(hostname, argv[optind], MAXHOSTNAMELEN);
    hostname[MAXHOSTNAMELEN] = 0;

    memset(&hints_numeric, 0, sizeof(hints_numeric));
    hints_numeric.ai_flags = AI_NUMERICHOST;
    hints_numeric.ai_socktype = SOCK_RAW;
    hints_numeric.ai_protocol = IPPROTO_RAW;
    if (strcmp(source_addr, "") != 0) {
        source = malloc(sizeof(struct addrinfo));
        status = getaddrinfo(source_addr, NULL, &hints_numeric, &source);
        if (status) {
            fprintf(stderr, "Not an IP address: %s\n", source_addr);
            abort();
        }
    } else {
        source = NULL;
    }

    memset(&hints, 0, sizeof(hints));
    if (v4_flag) {
        hints.ai_family = AF_INET;
    } else if (v6_flag) {
        hints.ai_family = AF_INET6;
    } else {
        hints.ai_family = AF_UNSPEC;
    }
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_RAW;
    result = malloc(sizeof(struct addrinfo));
    status = getaddrinfo(hostname, NULL, &hints_numeric, &result);
    if (status && status == EAI_NONAME) {       /* It may be a name, but it is
                                                 * certainly not an address */
        status = getaddrinfo(hostname, NULL, &hints, &result);
        if (status) {
            fprintf(stderr, "Nothing found about host name %s\n", hostname);
            abort();
        }
    }
    if (result->ai_family != AF_INET && result->ai_family != AF_INET6) {
        fprintf(stderr, "Only IPv4 and IPv6: family %i unsupported\n",
                result->ai_family);
        abort();
    }
    if (verbose_flag) {
        fprintf(stdout, "Connecting to %s...\n", text_of(result->ai_addr));
    }
    sd = socket(result->ai_family, SOCK_RAW, IPPROTO_RAW);
    if (sd < 0) {
        fprintf(stderr, "Cannot create raw socket: %s\n", strerror(errno));
        abort();
    }
    if (source != NULL
        && ((result->ai_family == AF_INET && source->ai_family != AF_INET)
            || (result->ai_family == AF_INET6 && source->ai_family != AF_INET6))) {
        fprintf(stderr,
                "The IP source address you specified (%s) must be the same IP address family than the target\n",
                source_addr);
        abort();
    }
    if (result->ai_family == AF_INET) {
        if (setsockopt(sd, IPPROTO_IP, IP_HDRINCL, (char *) &on, sizeof(on)) < 0) {
            fprintf(stderr, "Cannot set IP_HDRINCL: %s\n", strerror(errno));
            abort();
        }
    }
    /* We do not activate IP_HDRINCL for IPv6 sockets, it is undefined. On FreeBSD
     * or NetBSD, you'll receive an "Invalid argument". On Linux, it pretends to
     * work, but just because there is another option with the same number. Thanks
     * to Mathieu Peresse for his detailed explanations there. */
    if (port == 0) {
        port = PORT;
    }
    if (result->ai_family == AF_INET) {
        sockaddr4 = (struct sockaddr_in *) result->ai_addr;
        memset(&op4.ip, '\0', sizeof(op4.ip));
        op4.ip.ip_v = 4;
        op4.ip.ip_hl = sizeof(op4.ip) >> 2;
        op4.ip.ip_dst = sockaddr4->sin_addr;
        if (source == NULL) {
            /* Do nothing. At least on Linux, source is set automatically */
        } else {
            sockaddr4 = (struct sockaddr_in *) source->ai_addr;
            op4.ip.ip_src = sockaddr4->sin_addr;
        }
        op4.ip.ip_p = SOL_UDP;
        op4.ip.ip_ttl = TTL;
        headersize = sizeof(op4.ip) + sizeof(op4.udp);
        packetsize = headersize + strlen(message);
        op4.ip.ip_len = htons(packetsize);
        memset(&op4.udp, '\0', sizeof(op4.udp));
        op4.udp.uh_dport = htons(port);
        op4.udp.uh_sport = htons(port);
        op4.udp.uh_ulen = htons(strlen(message) + sizeof(op4.udp));
        op4.udp.uh_sum = 0;     /* TODO: we should put a real checksum here. On
                                 * Linux, this is done automatically but not on
                                 * NetBSD */
        memset(op4.payload, '\0', MSGSIZE);
        strcpy(op4.payload, message);
        packet = &op4;
        extraipheadersize = 0;
    } else if (result->ai_family == AF_INET6) {
        sockaddr6 = (struct sockaddr_in6 *) result->ai_addr;
        memset(&op6.ip, '\0', sizeof(op6.ip));
        op6.ip.ip6_vfc = 6 << 4;
        op6.ip.ip6_dst = sockaddr6->sin6_addr;
        if (source == NULL) {
            /* Do nothing. Unfortunately, at least on Linux,I have to set the
             * source, it is not done automatically (it works on NetBSD) */
        } else {
            sockaddr6 = (struct sockaddr_in6 *) source->ai_addr;
            op6.ip.ip6_src = sockaddr6->sin6_addr;
        }
        op6.ip.ip6_nxt = SOL_UDP;
        op6.ip.ip6_hlim = TTL;
        headersize = sizeof(op6.ip) + sizeof(op6.udp);
        packetsize = headersize + strlen(message);
        op6.ip.ip6_plen = htons((uint16_t) packetsize);
        memset(&op6.udp, '\0', sizeof(op6.udp));
        op6.udp.uh_dport = htons(port);
        op6.udp.uh_sport = htons(port);
        op6.udp.uh_ulen = htons(strlen(message) + sizeof(op6.udp));
        op6.udp.uh_sum = 0;
        memset(op6.payload, '\0', MSGSIZE);
        strcpy(op6.payload, message);
        packet = &op6;
        extraipheadersize = sizeof(op6.ip);
    }
    if (verbose_flag) {
        fprintf(stdout, "Sending %i bytes (%i for the headers)...\n",
                (int) packetsize, (int) headersize);
    }
    /* TODO: with IPv4, fails on FreeBSD with "Invalid argument" */
    num =
        sendto(sd, packet, packetsize + extraipheadersize, 0, result->ai_addr,
               result->ai_addrlen);
    if (num < 0) {
        fprintf(stderr, "Cannot send message:  %s\n", strerror(errno));
        abort();
    }
    return (0);
}
