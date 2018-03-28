#include	"db.h"

/* Fetch a specified record.
 * We return a pointer to the null-terminated data. */

char *
db_fetch(DB *db, const char *key)
{
	char	*ptr;

	if (_db_find(db, key, 0) < 0) {
		ptr = NULL;				/* error, record not found */
		db->cnt_fetcherr++;
	} else {
		ptr = _db_readdat(db);	/* return pointer to data */
		db->cnt_fetchok++;
	}
			/* Unlock the hash chain that _db_find() locked */
	if (un_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
		err_dump("un_lock error");
	return(ptr);
}
