/* protoent.c:
 * 
 * Example getprotoent(3) program :
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

int
main(int argc,char **argv) {
    int x;
    struct protoent *pp;

    for (;;) {
        errno = 0;
        if ( !(pp = getprotoent()) )
            break;

        printf("%s:\n"
            "\tProtocol: %d\n"
            "\tAliases:  ",
            pp->p_name,
            pp->p_proto);
        for ( x=0; pp->p_aliases[x] != NULL; ++x )
            printf("%s ",pp->p_aliases[x]);
        putchar('\n');
    }

    if ( errno != 0 
    &&   errno != ENOENT ) /* For RH-6.0 */
        fprintf(stderr,
            "%s: getprotoent(3) %d\n",
            strerror(errno),errno);

    return 0;
}
