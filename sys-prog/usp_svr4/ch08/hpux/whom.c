#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <utmp.h>
#include <pwd.h>

int
main(void)
{
    char name[64];
    struct utmp *utmp;
    struct passwd *pwd;
    struct hostent *hp;

    printf("Login    Name             Line     Time             Host\n");
    printf("--------------------------------------------------------\n");

    /*
     * Read each entry from the file.
     */
    while ((utmp = getutent()) != NULL) {
        /*
         * Skip records that aren't logins.
         */
        if (utmp->ut_type != USER_PROCESS)
            continue;

        /*
         * Get the real name.
         */
        if ((pwd = getpwnam(utmp->ut_user)) != NULL)
            strcpy(name, pwd->pw_gecos);
        else
            sprintf(name, "#%d", pwd->pw_uid);
    
        /*
         * Print stuff out.
         */
        printf("%-8s %-16.16s %-8.8s %.12s", utmp->ut_user, name,
               utmp->ut_line,
               ctime(&utmp->ut_time)+4);
        
        /*
         * If there's a remote host, get its name and print it.  The
         * gethostbyaddr() function is described in Chapter 14,
         * Networking With Sockets.
         */
        if (utmp->ut_addr != 0) {
                hp = gethostbyaddr((char *) &utmp->ut_addr, sizeof(long),
                        AF_INET);
                
                if (hp != NULL)
                        printf(" %s", hp->h_name);
        }

        putchar('\n');
    }

    exit(0);
}
