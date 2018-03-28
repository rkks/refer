/*

[The following solution is in the zip file as krx80401.c - RJH (ed.) ]

EXERCISE 8-4

I thought I'd improve 8-4 too.  I'm trying my best to get this as close
to ISO C as possible given the restrictions that I'm under.  (A real
implementation would have fsetpos() borrow some of the same code.)

*/

/* Gregory Pietsch -- My category 0 solution to 8-4 */

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

int fseek(FILE *f, long offset, int whence)
{
    int result;

    if ((f->flag & _UNBUF) == 0 && base != NULL) {
        /* deal with buffering */
        if (f->flag & _WRITE) {
            /* writing, so flush buffer */
            if (fflush(f))
                return EOF;  /* from 8-3 */
        } else if (f->flag & _READ) {
            /* reading, so trash buffer --
             * but I have to do some housekeeping first
             */
            if (whence == SEEK_CUR) {
                /* fix offset so that it's from the last 
                 * character the user read (not the last
                 * character that was actually read)
                 */
                if (offset >= 0 && offset <= f->cnt) {
                    /* easy shortcut */
                    f->cnt -= offset;
                    f->ptr += offset;
                    f->flags &= ~_EOF; /* see below */
                    return 0;
                } else
                    offset -= f->cnt;
            }
            f->cnt = 0;
            f->ptr = f->base;
        }
    }
    result = (lseek(f->fd, offset, whence) < 0);
    if (result == 0)
        f->flags &= ~_EOF; /* if successful, clear EOF flag */
    return result;
}

