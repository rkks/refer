#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>

#define SOCKETNAME  "mysocket"

int
main(void)
{
    int n, s, len;
    char buf[1024];
    struct sockaddr_un name;

    /*
     * Create a socket in the UNIX
     * domain.
     */
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    /*
     * Create the address of the server.
     */
    memset(&name, 0, sizeof(struct sockaddr_un));

    name.sun_family = AF_UNIX;
    strcpy(name.sun_path, SOCKETNAME);
    len = sizeof(name.sun_family) + strlen(name.sun_path);

    /*
     * Connect to the server.
     */
    if (connect(s, (struct sockaddr *) &name, len) < 0) {
        perror("connect");
        exit(1);
    }

    /*
     * Read from standard input, and copy the
     * data to the socket.
     */
    while ((n = read(0, buf, sizeof(buf))) > 0) {
        if (send(s, buf, n, 0) < 0) {
            perror("send");
            exit(1);
        }
    }

    close(s);
    exit(0);
}
