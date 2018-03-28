#include	"db.h"

/* Return the next sequential record.
 * We just step our way through the index file, ignoring deleted
 * records.  db_rewind() must be called before this is function
 * is called the first time.
 */

char *
db_nextrec(DB *db, char *key)
{
	char	c, *ptr;

		/* We read lock the free list so that we don't read
		   a record in the middle of its being deleted. */
	if (readw_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		err_dump("readw_lock error");

	do {
			/* read next sequential index record */
		if (_db_readidx(db, 0) < 0) {
			ptr = NULL;		/* end of index file, EOF */
			goto doreturn;
		}
			/* check if key is all blank (empty record) */
		ptr = db->idxbuf;
		while ( (c = *ptr++) != 0  &&  c == ' ')
			;	/* skip until null byte or nonblank */
	} while (c == 0);	/* loop until a nonblank key is found */

	if (key != NULL)
		strcpy(key, db->idxbuf);	/* return key */
	ptr = _db_readdat(db);	/* return pointer to data buffer */

	db->cnt_nextrec++;
doreturn:
	if (un_lock(db->idxfd, FREE_OFF, SEEK_SET, 1) < 0)
		err_dump("un_lock error");

	return(ptr);
}
