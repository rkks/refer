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
#ifndef _UXTERMINAL_HPP_
#define _UXTERMINAL_HPP_

#include <termios.h>

namespace Ux {

/**
	\ingroup Ux
*/
class Termios : public termios, public Base {

public:
	speed_t cfgetispeed(void);
	speed_t cfgetospeed(void);
	void cfsetispeed(speed_t speed);
	void cfsetospeed(speed_t speed);
};

/**
	\ingroup Ux
*/
class Terminal : public File {

public:
	Terminal(int f = -1, const char *p = NULL, ssize_t s = -1)
		: File(f, p, s)
		{ }
	Terminal(const char *p, ssize_t s = -1)
		: File(-1, p, s)
		{ }
	bool			isatty(void);
	static char *	ctermid(char *buf = NULL);
	void			tcdrain(void);
	void			tcflow(int action);
	void			tcflush(int queue);
	void			tcgetattr(Termios& t);
	pid_t			tcgetpgrp(void);
	pid_t			tcgetsid(void);
	void			tcsendbreak(int duration);
	void			tcsetattr(int actions, const Termios& t);
	void			tcsetpgrp(pid_t pgid);
	void			ttyname(char *buf, size_t bufsize);
};

} // namespace

#endif // _UXTERMINAL_HPP_
