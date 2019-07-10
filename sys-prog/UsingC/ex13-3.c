
#include	<sys/param.h>
#include	<sys/time.h>
#include	<sys/vnode.h>
#include	<ufs/inode.h>
#include	<ufs/fs.h>

#define sblock    sb_un.u_sblock

/*
 * The super block.  We allow enough room for
 * a complete disk block.
 */
union {
    struct  fs u_sblock;
    char    dummy[SBSIZE];
} sb_un;

read_blocks(dp)
struct dinode *dp;
{
    int count;
    register int i, n;
    char dblock[MAXBSIZE];

    count = dp->di_size;

    /*
     * For each direct block in the file (NDADDR indicates
     * the number of direct addresses stored)...
     */
    for (i = 0; (i < NDADDR) && (count > 0); i++) {
        /* 
         * Read in the block from disk.  Read in count
         * bytes or a disk block, whichever is less.
         */
        bread(fsbtodb(&sblock, dp->di_db[i]), dblock,
              n = min(count, sblock.fs_bsize));
        count -= n;

        /* process data block ... */
   
    }

    /* 
     * Now start reading the indirect blocks.  NIADDR is 
     * the number of indirect addresses.  Recall that 
     * the first indirect address is singly indirect,
     * the second is doubly indirect, an so on.
     */
    for (i = 0; (i < NIADDR) && (count > 0); i++)
        read_indirect (dp->di_ib[i], i, &count);
}

/*
 * read_indirect--read the indirect blocks of the file.  The
 * level argument indicates our level of indirection; 0 is 
 * singly indirect, 1 is doubly indirect, and so on.
 */
read_indirect (blkno, level, count)
ino_t blkno;
int *count;
int level;
{
    register int i, n;
    char dblock[MAXBSIZE];
    daddr_t idblk[MAXBSIZE / sizeof(daddr_t)];

    /*
     * Read in the block from disk.
     */
    if (blkno)
        bread(fsbtodb(&sblock, blkno), idblk, sblock.fs_bsize);
     else
        bzero(idblk, sblock.fs_bsize);

    /* 
     * If level is zero, then this block contains disk block
     * addresses, since blkno was a singly indirect address.
     * If level is non-zero, then this block contains addresses
     * of more indirect blocks.
     */
    if (level <= 0) {
        /*
         * For each disk block (the NINDIR macro returns
         * the number of indirect addresses in a block)...
         */
        for (i = 0; (i < NINDIR(&sblock)) && (*count > 0); i++) {
            /*
             * Read in the block from disk.
             */
            bread(fsbtodb(&sblock, idblk[i]), dblock,
                  n = min(*count, sblock.fs_bsize));
            *count -= n;

            /* process data block ... */
      }

      /*
       * Done processing.
       */
      return;
  }

     /*
      * Decrement the level we're at.
      */
     level--;

     /*
      * Handle the next level of indirection by calling
      * ourselves recursively with each address in this
      * block.
      */
     for (i = 0; i < NINDIR(&sblock); i++)  
         read_indirect(idblk[i], level, count);
}

