/*
	Synchronize processes with pipe
	AUP2, Sec. 6.02, 9.02.3 (not in book)

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "defs.h"
#include "pcsync.h"

bool pcsync_init(struct pcsync *p)
{
	ec_neg1( pipe(p->fdpc) )
	ec_neg1( pipe(p->fdcp) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool pcsync_wait_for_parent(struct pcsync *p)
{
	fd_set fdset;
#if 1
	ec_neg1( write(p->fdcp[1], "x", 1) )
	FD_ZERO(&fdset);
	FD_SET(p->fdpc[0], &fdset);
	ec_neg1( select(FD_SETSIZE, &fdset, NULL, NULL, NULL) )
#else
#endif
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool pcsync_unblock_children(struct pcsync *p, int children,
  pid_t *pid_child)
{
	int n;
	char c;

#if 1
	for (n = 0; n < children; n++)
		switch (read(p->fdcp[0], &c, 1)) {
		case 0: /* impossible */
			errno = 0;
			/* fall through */
		case -1:
			EC_FAIL
		}
	ec_neg1( write(p->fdpc[1], "x", 1) )
#endif
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}

bool pcsync_end(struct pcsync *p)
{
	(void)close(p->fdpc[0]);
	(void)close(p->fdpc[1]);
	(void)close(p->fdcp[0]);
	(void)close(p->fdcp[1]);
	return true;
}

