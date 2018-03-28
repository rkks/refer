#include <sys/types.h>
#include <unistd.h>

int
main(void)
{
    int i;
    char c;
    pid_t pid;

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
        for (i=0; i < 10; i++) {
            for (c = 'a'; c <= 'z'; c++)
                write(1, &c, 1);
        }
    }
    else {
        /*
         * This code executes in the parent process.
         */
        for (i=0; i < 10; i++) {
            for (c = 'A'; c <= 'Z'; c++)
                write(1, &c, 1);
        }
    }

    /*
     * This code executes in both processes (i.e.,
     * it gets executed twice).
     */
    write(1, "\n", 1);
    exit(0);
}
