#include <sys/types.h>
#include <unistd.h>

int
main(void)
{
    pid_t pid;
    char *args[4];

    /*
     * Create a child process.
     */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        /*
         * This code executes in the child process
         * (fork returned zero).
         */
        execl("/bin/echo", "echo", "Today's", "date", "is:", 0);

        /*
         * If the exec succeeds, we'll never get here.
         */
        perror("exec");
        exit(1);
    }


    /*
     * This code executes in the parent process.
     */
    args[0] = "date";
    args[1] = "+%A, %B %d, %Y";
    args[2] = NULL;

    execv("/bin/date", args);

    /*
     * If the exec succeeds, we'll never get here.
     */
    perror("exec");
    exit(1);
}
