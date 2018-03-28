#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int
main(int argc, char **argv)
{
    int n, in, out;
    char buf[1024];

    if (argc != 3) {
        write(2, "Usage: append file1 file2\n", 26);
        exit(1);
    }

    /*
     * Open the first file for reading.
     */
    if ((in = open(argv[1], O_RDONLY)) < 0) {
        perror(argv[1]);
        exit(1);
    }

    /*
     * Open the second file for writing.
     */
    if ((out = open(argv[2], O_WRONLY | O_APPEND)) < 0) {
        perror(argv[2]);
        exit(1);
    }

    /*
     * Copy data from the first file to the second.
     */
    while ((n = read(in, buf, sizeof(buf))) > 0)
        write(out, buf, n);

    close(out);
    close(in);
    exit(0);
}
