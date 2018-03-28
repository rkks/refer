#include	"db.h"

/* Read the current data record into the data buffer.
 * Return a pointer to the null-terminated data buffer. */

char *
_db_readdat(DB *db)
{
	if (lseek(db->datfd, db->datoff, SEEK_SET) == -1)
		err_dump("lseek error");

	if (read(db->datfd, db->datbuf, db->datlen) != db->datlen)
		err_dump("read error");
	if (db->datbuf[db->datlen - 1] != '\n')		/* sanity check */
		err_dump("missing newline");
	db->datbuf[db->datlen - 1] = 0;		/* replace newline with null */

	return(db->datbuf);		/* return pointer to data record */
}
