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
     * Remove any previous FIFO.
     */
    unlink(FIFONAME);

    /*
     * Create the FIFO.
     */
    if (mkfifo(FIFONAME, 0666) < 0) {
        perror("mkfifo");
        exit(1);
    }

    /*
     * Open the FIFO for reading.
     */
    if ((fd = open(FIFONAME, O_RDONLY)) < 0) {
        perror("open");
        exit(1);
    }

    /*
     * Read from the FIFO until end-of-file and
     * print what we get on the standard output.
     */
    while ((n = read(fd, buf, sizeof(buf))) > 0)
        write(1, buf, n);

    close(fd);
    exit(0);
}
