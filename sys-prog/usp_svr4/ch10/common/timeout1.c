#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int     flag = 0;

void    handler(int);

int
main(void)
{
    char buf[BUFSIZ];
    char *defstring = "hello";

    /*
     * Set up a timeout of 10 seconds.
     */
    signal(SIGALRM, handler);
    alarm(10);

    /*
     * Prompt for a string and remove the newline. 
     */
    printf("Enter a string: ");
    fgets(buf, sizeof(buf), stdin);
    buf[strlen(buf)-1] = '\0';

    /*
     * Turn off the alarm, they typed something.
     */
    alarm(0);

    /*
     * If flag is 1, the alarm went off.  Assume default string.
     */
    if (flag == 1) {
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
 * handler - catch alarm signal and set flag.
 */
void
handler(int sig)
{
    flag = 1;
}
