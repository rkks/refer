#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#define MAP_FAILED      ((void *) -1)

int
main(int argc, char **argv)
{
    int fd;
    struct stat st;
    caddr_t base, ptr;

    /*
     * For each file specified...
     */
    while (--argc) {
        /*
         * Open the file.
         */
        if ((fd = open(*++argv, O_RDONLY, 0)) < 0) {
            perror(*argv);
            continue;
        }

        /*
         * Find out how big the file is.
         */
        fstat(fd, &st);

        /*
         * Map the entire file into memory.
         */
        base = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

        if (base == MAP_FAILED) {
            perror(*argv);
            close(fd);
            continue;
        }

        /*
         * We can close the file now; we can access it
         * through memory.
         */
        close(fd);

        /*
         * Now print the file.
         */
        for (ptr = base; ptr < &base[st.st_size]; ptr++)
            putchar(*ptr);

        /*
         * Now unmap the file.
         */
        munmap(base, st.st_size);
    }

    exit(0);
}
