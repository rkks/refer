#define _GNU_SOURCE
#include <sys/wait.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Stack size for child process */
#define STACK_SIZE (1024 * 1024)

/* List of namesapces to unshare */
#define UNSHARED_NAMESPACES     \
    CLONE_NEWNET                \
    | CLONE_NEWIPC              \
    | CLONE_NEWPID              \
    | CLONE_NEWUTS              \
    | CLONE_NEWNS               \
    | CLONE_NEWUSER

char *newroot = NULL, *cmd = NULL;
char **args = NULL;

/* This function will be called in the 'cloned' process */
int child_func (void *arg) {
    int status;

    /* Change the root directory */
    if ((status = chroot(newroot))) {
        perror("chroot");
        return status;
    }

    /* Execute command with namespace unshared */
    if ((status = execv(cmd, args))) {
        perror("exec");
        return status;
    }
}

int main(int argc, char **argv) {
    char *stack;
    char *stack_top;
    pid_t child_pid;

    if (argc < 3) {
        printf("Usage: %s <new root> <command> [args .. ]\n", argv[0]);
        return 1;
    }

    /* Set up the parameters used when launching the container */
    newroot = argv[1];
    cmd = argv[2];
    args = calloc(argc - 1, sizeof(char *));

    if (args) {
        args[0] = cmd;
        memcpy(args + 1, argv + 3, (argc - 3) * sizeof(char *));
    } else {
        perror("malloc");
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
                           UNSHARED_NAMESPACES | SIGCHLD,
                           argv+1)) == -1) {
        perror("clone");
        return 1;
    }

    /* Wait for child to exit */
    if (waitpid(child_pid, NULL, 0) == -1) {
        perror("waitpid");
        return 1;
    }
}
