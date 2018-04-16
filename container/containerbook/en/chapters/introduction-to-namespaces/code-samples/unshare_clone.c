#define _GNU_SOURCE
#include <sys/wait.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Stack size for child process */
#define STACK_SIZE (1024 * 1024)

/* This function will be called in the 'cloned' process */
int child_func (void *arg) {
    int status;
    char *cmd = (char *)arg;

    /* Execute command with namespace unshared */
    if ((status = execl(arg, arg, NULL))) {
        perror("exec");
        return status;
    }
}

int main(int argc, char **argv) {
    char *stack;
    char *stack_top;
    pid_t child_pid;

    if (argc != 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    /* Allocate some memory for the child stack */
    if ((stack = malloc(STACK_SIZE)) == NULL) {
        perror("malloc");
        return 1;
    }

    stack_top = stack + STACK_SIZE; /* Stack grows downward */

    /* Clone with a new network namespace */
    if ((child_pid = clone(child_func,
                           stack_top,
                           CLONE_NEWNET | SIGCHLD,
                           argv[1])) == -1) {
        perror("clone");
        return 1;
    }

    /* Wait for child to exit */
    if (waitpid(child_pid, NULL, 0) == -1) {
        perror("waitpid");
        return 1;
    }
}
