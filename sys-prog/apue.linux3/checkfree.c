#include	"db.h"

/* Check the consistency of the free list, and return a count of
   the number of records on the free list.
   This is an undocumented function and should be called when the
   db is not being updated. */

int
_db_checkfree(DB *db)
{
	off_t	offset, nextoffset, saveoffset;
	char	c, *ptr;
	int		count;

		/* Read the free list pointer */
	saveoffset = FREE_OFF;
	offset = _db_readptr(db, saveoffset);

		/* Loop through the free list */
	count = 0;
	while (offset != 0) {
		count++;
		nextoffset = _db_readidx(db, offset);

			/* make certain key is all blank */
		ptr = db->idxbuf;
		while ( (c = *ptr++) != 0)
			if (c != ' ')
				return(-1);	/* error */

			/* make certain data is all blank */
		ptr = _db_readdat(db);
		while ( (c = *ptr++) != 0)
			if (c != ' ')
				return(-1);	/* error */

		saveoffset = offset;
		offset = nextoffset;
	}
	return(count);		/* can be zero */
}
