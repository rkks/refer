#include <sys/types.h>
#include <sys/time.h>
#include <utmpx.h>
#include <pwd.h>

int
main(void)
{
    char name[64];
    struct passwd *pwd;
    struct utmpx *utmpx;

    printf("Login    Name             Line     Time             Host\n");
    printf("--------------------------------------------------------\n");

    /*
     * Read each entry from the file.
     */
    while ((utmpx = getutxent()) != NULL) {
        /*
         * Skip records that aren't logins.
         */
        if (utmpx->ut_type != USER_PROCESS)
            continue;

        /*
         * Get the real name.
         */
        if ((pwd = getpwnam(utmpx->ut_user)) != NULL)
            strcpy(name, pwd->pw_gecos);
        else
            sprintf(name, "#%d", pwd->pw_uid);
    
        /*
         * Print stuff out.
         */
        printf("%-8s %-16.16s %-8.8s %.12s", utmpx->ut_user, name,
               utmpx->ut_line, ctime(&utmpx->ut_tv.tv_sec)+4);
        
        if (utmpx->ut_syslen > 0)
            printf(" %s", utmpx->ut_host);
        
        putchar('\n');
    }

    exit(0);
}
