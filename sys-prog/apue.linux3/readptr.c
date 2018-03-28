#include	"db.h"

/* Read a chain ptr field from anywhere in the index file:
 * the free list pointer, a hash table chain ptr, or an
 * index record chain ptr.  */

off_t
_db_readptr(DB *db, off_t offset)
{
	char	asciiptr[PTR_SZ + 1];

	if (lseek(db->idxfd, offset, SEEK_SET) == -1)
		err_dump("lseek error to ptr field");
	if (read(db->idxfd, asciiptr, PTR_SZ) != PTR_SZ)
		err_dump("read error of ptr field");

	asciiptr[PTR_SZ] = 0;		/* null terminate */
	return(atol(asciiptr));
}
