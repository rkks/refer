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
#ifndef _UXPTY_HPP_
#define _UXPTY_HPP_

namespace Ux {

/**
	\ingroup Ux
	Note that ioctl is not included because there's no portable way to handle the arguments
	with stdargs (variable arguments). The Pseudo-Terminal Library from AUP2 has not been included,
	although it probably ought to be, as it's much easier to use than the low-level functions, and
	more portable.
*/
class Pty : public Terminal {

public:
	Pty(int f = -1, const char *p = NULL, ssize_t s = -1)
		: File(f, p, s)
		{ }
	Pty(const char *p, ssize_t s = -1)
		: File(-1, p, s)
		{ }
	void posix_openpt(int oflag = O_RDWR);
	void grantpt(void);
	void unlockpt(void);
	char *ptsname(void);
};

} // namespace

#endif // _UXPTY_HPP_
