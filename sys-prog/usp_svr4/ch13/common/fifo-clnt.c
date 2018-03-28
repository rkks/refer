#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFONAME    "myfifo"

int
main(void)
{
    int n, fd;
    char buf[1024];

    /*
     * Open the FIFO for writing.  It was
     * created by the server.
     */
    if ((fd = open(FIFONAME, O_WRONLY)) < 0) {
        perror("open");
        exit(1);
    }

    /*
     * Read from standard input, and copy the
     * data to the FIFO.
     */
    while ((n = read(0, buf, sizeof(buf))) > 0)
        write(fd, buf, n);

    close(fd);
    exit(0);
}
