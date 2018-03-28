#include	"db.h"

void
db_close(DB *db)
{
	_db_free(db);	/* closes fds, free buffers & struct */
}
