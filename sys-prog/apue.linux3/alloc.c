#include	"db.h"

/* Allocate & initialize a DB structure, and all the buffers it needs  */

DB *
_db_alloc(int namelen)
{
	DB		*db;

			/* Use calloc, to init structure to zero */
	if ( (db = calloc(1, sizeof(DB))) == NULL)
		err_dump("calloc error for DB");

	db->idxfd = db->datfd = -1;				/* descriptors */

		/* Allocate room for the name.
		   +5 for ".idx" or ".dat" plus null at end. */

	if ( (db->name = malloc(namelen + 5)) == NULL)
		err_dump("malloc error for name");

		/* Allocate an index buffer and a data buffer.
		   +2 for newline and null at end. */

	if ( (db->idxbuf = malloc(IDXLEN_MAX + 2)) == NULL)
		err_dump("malloc error for index buffer");
	if ( (db->datbuf = malloc(DATLEN_MAX + 2)) == NULL)
		err_dump("malloc error for data buffer");

	return(db);
}
