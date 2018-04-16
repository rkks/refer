#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int status;

    if (argc != 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    /* Unshare network namespace */
    if ((status = unshare(CLONE_NEWNET))) {
        perror("unshare");
        return status;
    }

    /* Execute command with namespace unshared */
    if ((status = execl(argv[1], argv[1], NULL))) {
        perror("exec");
        return status;
    }
}
