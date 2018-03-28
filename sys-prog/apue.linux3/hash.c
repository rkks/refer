#include	"db.h"

/* Calculate the hash value for a key. */

hash_t
_db_hash(DB *db, const char *key)
{
	hash_t		hval;
	const char	*ptr;
	char		c;
	int			i;

	hval = 0;
	for (ptr = key, i = 1; c = *ptr++; i++)
		hval += c * i;		/* ascii char times its 1-based index */

	return(hval % db->nhash);
}
