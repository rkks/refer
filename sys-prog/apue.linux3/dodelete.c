#include	"db.h"

/* Delete the current record specified by the DB structure.
 * This function is called by db_delete() and db_store(),
 * after the record has been located by _db_find(). */

int
_db_dodelete(DB *db)
{
	int		i;
	char	*ptr;
	off_t	freeptr, saveptr;

		/* Set data buffer to all blanks */
	for (ptr = db->datbuf, i = 0; i < db->datlen - 1; i++)
		*ptr++ = ' ';
	*ptr = 0;	/* null terminate for _db_writedat() */

		/* Set key to blanks */
	ptr = db->idxbuf;
	while (*ptr)
		*ptr++ = ' ';

		/* We have to lock the free list */
	if (writew_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		err_dump("writew_lock error");

		/* Write the data record with all blanks */
	_db_writedat(db, db->datbuf, db->datoff, SEEK_SET);

		/* Read the free list pointer.  Its value becomes the
		   chain ptr field of the deleted index record.  This means
		   the deleted record becomes the head of the free list. */
	freeptr = _db_readptr(db, FREE_OFF);

		/* Save the contents of index record chain ptr,
		   before it's rewritten by _db_writeidx(). */
	saveptr = db->ptrval;

		/* Rewrite the index record.  This also rewrites the length
		   of the index record, the data offset, and the data length,
		   none of which has changed, but that's OK. */
	_db_writeidx(db, db->idxbuf, db->idxoff, SEEK_SET, freeptr);

		/* Write the new free list pointer */
	_db_writeptr(db, FREE_OFF, db->idxoff);

		/* Rewrite the chain ptr that pointed to this record
		   being deleted.  Recall that _db_find() sets db->ptroff
		   to point to this chain ptr.  We set this chain ptr
		   to the contents of the deleted record's chain ptr,
		   saveptr, which can be either zero or nonzero. */
	_db_writeptr(db, db->ptroff, saveptr);

	if (un_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		err_dump("un_lock error");

	return(0);
}
