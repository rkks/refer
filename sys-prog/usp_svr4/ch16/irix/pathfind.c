#include <stdlib.h>
#include <libgen.h>

int
main(int argc, char **argv)
{
    char *p, *path;

    if ((path = getenv("PATH")) == NULL) {
        fprintf(stderr, "cannot find path in environment.\n");
        exit(1);
    }

    while (--argc) {
        if ((p = pathfind(path, *++argv, "x")) == NULL)
            printf("%s: not found in search path.\n", *argv);
        else
            printf("%s: %s\n", *argv, p);
    }

    exit(0);
}
