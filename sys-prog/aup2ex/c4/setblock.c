/*
	setblock (turn blocking on or off)
	AUP2, Sec. 4.02.2

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
#include "setblock.h"

/* turn blocking on or off */
/*[setblock]*/
bool setblock(int fd, bool block)
{
	int flags;

	ec_neg1( flags = fcntl(fd, F_GETFL) )
	if (block)
		flags &= ~O_NONBLOCK;
	else
		flags |= O_NONBLOCK;
	ec_neg1( fcntl(fd, F_SETFL, flags) )
	return true;

EC_CLEANUP_BGN
	return false;
EC_CLEANUP_END
}
/*[]*/
