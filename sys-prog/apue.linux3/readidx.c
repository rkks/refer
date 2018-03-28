#include	"db.h"
#include	<sys/uio.h>		/* struct iovec */

/* Read the next index record.  We start at the specified offset in
 * the index file.  We read the index record into db->idxbuf and
 * replace the separators with null bytes.  If all is OK we set
 * db->datoff and db->datlen to the offset and length of the
 * corresponding data record in the data file.  */

off_t
_db_readidx(DB *db, off_t offset)
{
	int				i;
	char			*ptr1, *ptr2;
	char			asciiptr[PTR_SZ + 1], asciilen[IDXLEN_SZ + 1];
	struct iovec	iov[2];

		/* Position index file and record the offset.  db_nextrec()
		   calls us with offset==0, meaning read from current offset.
		   We still need to call lseek() to record the current offset. */
	if ( (db->idxoff = lseek(db->idxfd, offset,
							 offset == 0 ? SEEK_CUR : SEEK_SET)) == -1)
		err_dump("lseek error");

		/* Read the ascii chain ptr and the ascii length at
		   the front of the index record.  This tells us the
		   remaining size of the index record. */
	iov[0].iov_base = asciiptr;
	iov[0].iov_len  = PTR_SZ;
	iov[1].iov_base = asciilen;
	iov[1].iov_len  = IDXLEN_SZ;
	if ( (i = readv(db->idxfd, &iov[0], 2)) != PTR_SZ + IDXLEN_SZ) {
		if (i == 0 && offset == 0)
			return(-1);		/* EOF for db_nextrec() */
		err_dump("readv error of index record");
	}

	asciiptr[PTR_SZ] = 0;			/* null terminate */
	db->ptrval = atol(asciiptr);	/* offset of next key in chain */
						/* this is our return value; always >= 0 */
	asciilen[IDXLEN_SZ] = 0;		/* null terminate */
	if ( (db->idxlen = atoi(asciilen)) < IDXLEN_MIN ||
							db->idxlen > IDXLEN_MAX)
		err_dump("invalid length");

		/* Now read the actual index record.  We read it into the key
		   buffer that we malloced when we opened the database. */
	if ( (i = read(db->idxfd, db->idxbuf, db->idxlen)) != db->idxlen)
		err_dump("read error of indexc record");
	if (db->idxbuf[db->idxlen-1] != '\n')
		err_dump("missing newline");	/* sanity checks */
	db->idxbuf[db->idxlen-1] = 0;		/* replace newline with null */

		/* Find the separators in the index record */
	if ( (ptr1 = strchr(db->idxbuf, SEP)) == NULL)
		err_dump("missing first separator");
	*ptr1++ = 0;				/* replace SEP with null */

	if ( (ptr2 = strchr(ptr1, SEP)) == NULL)
		err_dump("missing second separator");
	*ptr2++ = 0;				/* replace SEP with null */

	if (strchr(ptr2, SEP) != NULL)
		err_dump("too many separators");

		/* Get the starting offset and length of the data record */
	if ( (db->datoff = atol(ptr1)) < 0)
		err_dump("starting offset < 0");
	if ( (db->datlen = atol(ptr2)) <= 0 || db->datlen > DATLEN_MAX)
		err_dump("invalid length");
	return(db->ptrval);		/* return offset of next key in chain */
}
