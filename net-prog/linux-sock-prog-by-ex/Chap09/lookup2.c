/* lookup.c:
 * 
 * Example of gethostbyname(3),
 * using sethostent(3) :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

extern int h_errno;

#define TRUE    1
#define FALSE   0

int
main(int argc,char **argv) {
    int x, x2;
    struct hostent *hp;

    sethostent(TRUE);

    for ( x=1; x<argc; ++x ) {
        /*
         * Look up the host name :
         */
        hp = gethostbyname(argv[x]);
        if ( !hp ) {
            /* Report lookup failure */
            fprintf(stderr,
                "%s: host '%s'\n",
                hstrerror(h_errno),
                argv[x]);
            continue;
        }

        /*
         * Report the findings :
         */
        printf("Host %s :\n",argv[x]);
        printf("  Officially:\t%s\n",
            hp->h_name);
        fputs("  Aliases:\t",stdout);
        for ( x2=0; hp->h_aliases[x2]; ++x2 ) {
            if ( x2 )
                fputs(", ",stdout);
            fputs(hp->h_aliases[x2],stdout);
        }
        fputc('\n',stdout);
        printf("  Type:\t\t%s\n",
            hp->h_addrtype == AF_INET
                ? "AF_INET"
                : "AF_INET6");
        if ( hp->h_addrtype == AF_INET ) {
            for ( x2=0; hp->h_addr_list[x2]; ++x2 )
                printf("  Address:\t%s\n",
                    inet_ntoa( *(struct in_addr *)
                        hp->h_addr_list[x2]));
        }
        putchar('\n');
    }

    return 0;
}
