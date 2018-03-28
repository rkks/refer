#include	"db.h"

/* Delete the specified record */

int
db_delete(DB *db, const char *key)
{
	int		rc;

	if (_db_find(db, key, 1) == 0) {
		rc = _db_dodelete(db);	/* record found */
		db->cnt_delok++;
	} else {
		rc = -1;				/* not found */
		db->cnt_delerr++;
	}

	if (un_lock(db->idxfd, db->chainoff, SEEK_SET, 1) < 0)
		err_dump("un_lock error");
	return(rc);
}
