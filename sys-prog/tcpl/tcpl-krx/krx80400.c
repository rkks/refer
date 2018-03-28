/* Gregory Pietsch -- My category 0 solution to 8-4 */

int fseek(FILE *f, long offset, int whence)
{
    if ((f->flag & _UNBUF) == 0 && base != NULL) {
        /* deal with buffering */
        if (f->flag & _WRITE) {
            /* writing, so flush buffer */
            fflush(f);  /* from 8-3 */
        } else if (f->flag & _READ) {
            /* reading, so trash buffer */
            f->cnt = 0;
            f->ptr = f->base;
        }
    }
    return (lseek(f->fd, offset, whence) < 0);
}
