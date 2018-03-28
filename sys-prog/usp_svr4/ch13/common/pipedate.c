#include <sys/types.h>
#include <unistd.h>

struct {
    char    *abbrev;
    char    *fullname;
} days[] = {
    "Sun",  "Sunday",
    "Mon",  "Monday",
    "Tue",  "Tuesday",
    "Wed",  "Wednesday",
    "Thu",  "Thursday",
    "Fri",  "Friday",
    "Sat",  "Saturday",
    0,      0
};

int
main(void)
{
    pid_t pid;
    int pfd[2];
    int i, status;
    char line[64];

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
     * The child process executes "date".
     */
    if (pid == 0) {
        /*
         * Attach standard output to the pipe.
         */
        dup2(pfd[1], 1);
        close(pfd[0]);

        execl("/bin/date", "date", 0);
        perror("exec");
        _exit(127);
    }

    /*
     * We will not be writing to the pipe.
     */
    close(pfd[1]);

    /*
     * Read the output of "date".
     */
    if (read(pfd[0], line, 3) < 0) {
        perror("read");
        exit(1);
    }

    /*
     * For each day, see if it matches the output
     * from the date command.
     */
    for (i=0; days[i].abbrev != NULL; i++) {
        if (strncmp(line, days[i].abbrev, 3) == 0)
            printf("Today is %s.\n", days[i].fullname);
        else
            printf("Today is not %s.\n", days[i].fullname);
    }

    /*
     * Close the pipe and wait for the child
     * to exit.
     */
    close(pfd[0]);
    waitpid(pid, &status, 0);

    /*
     * Exit with a status of 0, indicating that
     * everything went fine.
     */
    exit(0);
}
