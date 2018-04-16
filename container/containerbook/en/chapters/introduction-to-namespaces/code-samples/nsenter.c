#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    int status;

    if (argc != 3) {
        printf("Usage: %s <namespace fd> <command>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (!fd) {
        perror("open");
        return 1;
    }

    /* Enter the namespace given as the first parameter */
    if ((status = setns(fd, 0))) {
        perror("setns");
        return status;
    }

    /* Execute command with namespace entered */
    if ((status = execl(argv[2], argv[2], NULL))) {
        perror("exec");
        return status;
    }
}
