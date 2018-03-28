#include <signal.h>
#include <unistd.h>
#include <setjmp.h>
#include <stdio.h>

jmp_buf env;

void    handler(int);

int
main(void)
{
    char buf[BUFSIZ];
    char *defstring = "hello";

    /*
     * Set up signal handler.
     */
    signal(SIGALRM, handler);

    /*
     * If setjmp returns 0, we're going through the first time.
     * Otherwise, we're going through after a longjmp.
     */
    if (setjmp(env) == 0) {
        /*
         * Set an alarm for 10 seconds.
         */
        alarm(10);

        /*
         * Prompt for a string and strip the newline.
         */
        printf("Enter a string: ");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf)-1] = '\0';

        /*
         * Turn off the alarm; they typed something.
         */
        alarm(0);
    }
    else {
        strcpy(buf, defstring);
        putchar('\n');
    }
    
    /*
     * Display the string we're using.
     */
    printf("Using string \"%s\"\n", buf);
    exit(0);
}

/*
 * handler - catch alarm signal and longjmp.
 */
void
handler(int sig)
{
    longjmp(env, 1);
}
