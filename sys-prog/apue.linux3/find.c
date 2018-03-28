#include	"db.h"

/* Find the specified record.
 * Called by db_delete(), db_fetch(), and db_store(). */

int
_db_find(DB *db, const char *key, int writelock)
{
	off_t	offset, nextoffset;

		/* Calculate hash value for this key, then calculate byte
		   offset of corresponding chain ptr in hash table.
		   This is where our search starts. */

			/* calc offset in hash table for this key */
	db->chainoff = (_db_hash(db, key) * PTR_SZ) + db->hashoff;
	db->ptroff = db->chainoff;

			/* Here's where we lock this hash chain.  It's the
			   caller's responsibility to unlock it when done.
			   Note we lock and unlock only the first byte. */
	if (writelock) {
		if (writew_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
			err_dump("writew_lock error");
	} else {
		if (readw_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
			err_dump("readw_lock error");
	}
			/* Get the offset in the index file of first record
			   on the hash chain (can be 0) */
	offset = _db_readptr(db, db->ptroff);

	while (offset != 0) {
		nextoffset = _db_readidx(db, offset);
		if (strcmp(db->idxbuf, key) == 0)
			break;		/* found a match */

		db->ptroff = offset;	/* offset of this (unequal) record */
		offset = nextoffset;	/* next one to compare */
	}
	if (offset == 0)
		return(-1);		/* error, record not found */

		/* We have a match.  We're guaranteed that db->ptroff contains
		   the offset of the chain ptr that points to this matching
		   index record.  _db_dodelete() uses this fact.  (The chain
		   ptr that points to this matching record could be in an
		   index record or in the hash table.) */
	return(0);
}
