#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int
main(void)
{
    pid_t pid;
    int pfd[2];
    int i, status;
    char *username;

    /*
     * Obtain the user name of the person
     * running this program.
     */
    if ((username = cuserid(NULL)) == NULL) {
        fprintf(stderr, "Who are you?\n");
        exit(1);
    }

    /*
     * Create a pipe.
     */
    if (pipe(pfd) < 0) {
        perror("pipe");
        exit(1);
    }

    /*
     * Create a child process.
     */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    /*
     * The child process executes "mail".
     */
    if (pid == 0) {
        /*
         * Attach standard input to the pipe.
         */
        dup2(pfd[0], 0);
        close(pfd[1]);

        execl("/bin/mail", "mail", username, 0);
        perror("exec");
        _exit(127);
    }

    /*
     * We won't be reading from the pipe.
     */
    close(pfd[0]);

    /*
     * Write our mail message to the pipe.
     */
    write(pfd[1], "Greetings and salutations,\n\n", 28);
    write(pfd[1], "This is your program saying hello.\n", 35);
    write(pfd[1], "Have a nice day.\n\n", 18);
    write(pfd[1], "Bye.\n", 5);

    /*
     * Close the pipe and wait for the child
     * to exit.
     */
    close(pfd[1]);
    waitpid(pid, &status, 0);

    /*
     * Exit with a status of 0, indicating that
     * everything went fine.
     */
    exit(0);
}
