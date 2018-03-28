/*
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
#ifndef _UXDIR_HPP_
#define _UXDIR_HPP_

#include <sys/uio.h>
#include <utime.h>

#include "uxfile.hpp"

namespace Ux {

/**
	\ingroup Ux
*/
class Dir : public File {
public:
	Dir(int f = -1, char *p = NULL, ssize_t s = -1)
		: File(f, p, s)
		{ }
	Dir(char *p, ssize_t s = -1)
		: File(-1, p, s)
		{ }

	void		mkdir(mode_t perms = PERM_DIRECTORY);
	void		rmdir(void);
};

} // namespace

#endif // _UXDIR_HPP_
