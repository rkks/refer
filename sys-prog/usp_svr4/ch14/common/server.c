#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORTNUMBER  12345

int
main(void)
{
    char buf[1024];
    int n, s, ns, len;
    struct sockaddr_in name;

    /*
     * Create the socket.
     */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    /*
     * Create the address of the server.
     */
    memset(&name, 0, sizeof(struct sockaddr_in));

    name.sin_family = AF_INET;
    name.sin_port = htons(PORTNUMBER);
    len = sizeof(struct sockaddr_in);

    /*
     * Use the wildcard address.
     */
    n = INADDR_ANY;
    memcpy(&name.sin_addr, &n, sizeof(long));

    /*
     * Bind the socket to the address.
     */
    if (bind(s, (struct sockaddr *) &name, len) < 0) {
        perror("bind");
        exit(1);
    }

    /*
     * Listen for connections.
     */
    if (listen(s, 5) < 0) {
        perror("listen");
        exit(1);
    }

    /*
     * Accept a connection.
     */
    if ((ns = accept(s, (struct sockaddr *) &name, &len)) < 0) {
        perror("accept");
        exit(1);
    }

    /*
     * Read from the socket until end-of-file and
     * print what we get on the standard output.
     */
    while ((n = recv(ns, buf, sizeof(buf), 0)) > 0)
        write(1, buf, n);

    close(ns);
    close(s);
    exit(0);
}
