/*
	Get block size
	AUP2, Sec. 3.2.3 (not in book)

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

#if _XOPEN_SOURCE >= 4
#include <sys/statvfs.h>
#define STATVFS_NAME statvfs

#elif defined(BSD_DERIVED)
#include <sys/param.h>
#include <sys/mount.h>
#define STATVFS_NAME statfs

#else
#error "Need statvfs or nonstandard substitute"
#endif

unsigned long getblksize(const char *path)
{
	struct STATVFS_NAME buf;

	if (path == NULL)
		path = ".";
	ec_neg1( STATVFS_NAME(path, &buf) )
	return buf.f_bsize;

EC_CLEANUP_BGN
	return 0;
EC_CLEANUP_END
}
