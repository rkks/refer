#include <sys/param.h>
#include <sys/time.h>
#include <sys/vnode.h>
#include <sys/fs/ufs_inode.h>
#include <sys/fs/ufs_fs.h>
#include <unistd.h>

int bread(int, daddr_t, char *, int);
int readDataBlocks(int, struct fs *, struct dinode *, int (*)(char *, int));
int readIndirect(int, struct fs *, daddr_t, int, int *, int (*)(char *, int));

int
readDataBlocks(int fd, struct fs *sblock, struct dinode *dp,
               int (*fn)(char *, int))
{
    int i, n, count;
    char block[MAXBSIZE];

    /*
     * Read the direct blocks.  There are NDADDR of them.
     */
    count = dp->di_size;

    for (i = 0; i < NDADDR && count > 0; i++) {
        /*
         * Read in the block from disk.
         */
        n = min(count, sblock->fs_bsize);

        if (bread(fd, fsbtodb(sblock, dp->di_db[i]), block, n) < 0)
            return(-1);

        count -= n;

        /*
         * Call the user's function on the block.
         */
        (*fn)(block, n);
    }

    /*
     * Now read the indirect blocks.  There are NIADDR of them.
     * Recall that the first address is a singly indirect block,
     * the second is a doubly indirect block, and so on.
     */
    for (i = 0; i < NIADDR && count > 0; i++) {
        if (readIndirect(fd, sblock, dp->di_ib[i], i, &count, fn) < 0)
            return(-1);
    }

    return(0);
}

int
readIndirect(int fd, struct fs *sblock, daddr_t blkno, int level, int *count,
             int (*fn)(char *, int))
{
    int i, n;
    char block[MAXBSIZE];
    daddr_t idblk[MAXBSIZE / sizeof(daddr_t)];

    /*
     * Read the block in from disk.
     */
    if (blkno)
        bread(fd, fsbtodb(sblock, blkno), (char *) idblk, sblock->fs_bsize);
    else
        memset(idblk, 0, sizeof(idblk));

    /*
     * If level is zero, then this block contains disk block
     * addresses (i.e., it's singly indirect).  If level is
     * non-zero, then this block contains addresses of more
     * indirect blocks.
     */
    if (level == 0) {
        /*
         * Read the disk blocks.  There are NINDIR
         * of them.
         */
        for (i = 0; i < NINDIR(sblock) && *count > 0; i++) {
            n = min(*count, sblock->fs_bsize);
            
            if (bread(fd, fsbtodb(sblock, idblk[i]), block, n) < 0)
                return(-1);

            *count -= n;

            /*
             * Call the user's function.
             */
            (*fn)(block, n);
        }
    }
    else {
        /*
         * Decrement the level.
         */
        level--;

        /*
         * Handle the next level of indirection by calling
         * ourselves recursively with each address in this
         * block.
         */
        for (i = 0; i < NINDIR(sblock); i++) {
            n = readIndirect(fd, sblock, idblk[i], level, count, fn);

            if (n < 0)
                return(-1);
        }
    }

    return(0);
}

/*
 * bread - read count bytes into buf, starting at disk block blockno.
 */
int
bread(int fd, daddr_t blockno, char *buf, int count)
{
    /*
     * Seek to the right place.
     */
    if (lseek(fd, (long) blockno * DEV_BSIZE, SEEK_SET) < 0) {
        perror("lseek");
        return(-1);
    }

    /*
     * Read in the data.
     */
    if ((count = read(fd, buf, count)) < 0) {
        perror("read");
        return(-1);
    }

    return(count);
}
