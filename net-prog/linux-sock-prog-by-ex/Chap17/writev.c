/* writev.c
 *
 * Short writev(2) demo:
 */
#include <sys/uio.h>

int
main(int argc,char **argv) {
    static char part2[] = "THIS IS FROM WRITEV";
    static char part3[] = "]\n";
    static char part1[] = "[";
    struct iovec iov[3];

    iov[0].iov_base = part1;
    iov[0].iov_len = strlen(part1);

    iov[1].iov_base = part2;
    iov[1].iov_len = strlen(part2);

    iov[2].iov_base = part3;
    iov[2].iov_len = strlen(part3);

    writev(1,iov,3);

    return 0;
}
