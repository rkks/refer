#include	"db.h"

/* Try to find a free index record and accompanying data record
 * of the correct sizes.  We're only called by db_store(). */

int
_db_findfree(DB *db, int keylen, int datlen)
{
	int		rc;
	off_t	offset, nextoffset, saveoffset;

		/* Lock the free list */
	if (writew_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		err_dump("writew_lock error");

		/* Read the free list pointer */
	saveoffset = FREE_OFF;
	offset = _db_readptr(db, saveoffset);

	while (offset != 0) {
		nextoffset = _db_readidx(db, offset);
		if (strlen(db->idxbuf) == keylen && db->datlen == datlen)
			break;		/* found a match */

		saveoffset = offset;
		offset = nextoffset;
	}

	if (offset == 0)
		rc = -1;	/* no match found */
	else {
			/* Found a free record with matching sizes.
			   The index record was read in by _db_readidx() above,
			   which sets db->ptrval.  Also, saveoffset points to
			   the chain ptr that pointed to this empty record on
			   the free list.  We set this chain ptr to db->ptrval,
			   which removes the empty record from the free list. */

		_db_writeptr(db, saveoffset, db->ptrval);
		rc = 0;

			/* Notice also that _db_readidx() set both db->idxoff
			   and db->datoff.  This is used by the caller, db_store(),
			   to write the new index record and data record. */
	}
			/* Unlock the free list */
	if (un_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		err_dump("un_lock error");
	return(rc);
}
