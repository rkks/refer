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
#include "ux.hpp"

using namespace Ux;

/**
	Calls cfgetispeed.
*/
speed_t Termios::cfgetispeed(void)
{
	return ::cfgetispeed(this);
}

/**
	Calls cfgetospeed.
*/
speed_t Termios::cfgetospeed(void)
{
	return ::cfgetospeed(this);
}

/**
	Calls cfsetispeed.
*/
void Termios::cfsetispeed(speed_t speed)
{
	if (::cfsetispeed(this, speed) == -1)
		throw Error(errno);
}

/**
	Calls cfsetospeed.
*/
void Termios::cfsetospeed(speed_t speed)
{
	if (::cfsetospeed(this, speed) == -1)
		throw Error(errno);
}

/**
	Calls isatty and changes return value to a bool.
*/
bool Terminal::isatty(void)
{
	return ::isatty(fd) == 1;
}

/**
	Calls ctermid. Static because makes no reference to fd -- returns name of controlling terminal.
*/
/* static */ char *Terminal::ctermid(char *buf)/* rtn pathname or empty string on error (errno not set) */
{
	char *s;

	errno = 0;
	s = ::ctermid(buf);
	if (s[0] == '\0') {
		if (errno == 0)
			throw Error(EINVAL);
		else
			throw Error(errno);
	}
	return s;
}

/**
	Calls tcdrain.
*/
void Terminal::tcdrain(void)
{
	if (::tcdrain(fd) == -1)
		throw Error(errno);
}

/**
	Calls tcflow.
*/
void Terminal::tcflow(int action)
{
	if (::tcflow(fd, action) == -1)
		throw Error(errno);
}

/**
	Calls tcflush.
*/
void Terminal::tcflush(int queue)
{
	if (::tcflush(fd, queue) == -1)
		throw Error(errno);
}

/**
	Calls tcgetattr.
*/
void Terminal::tcgetattr(Termios& t)
{
	if (::tcgetattr(fd, &t) == -1)
		throw Error(errno);
}

/**
	Calls tcgetpgrp.
*/
pid_t Terminal::tcgetpgrp(void)
{
	pid_t pid;
	
	if ((pid = ::tcgetpgrp(fd)) == -1)
		throw Error(errno);
	return pid;
}

/**
	Calls tcgetsid.
*/
pid_t Terminal::tcgetsid(void)
{
	pid_t pid;
	
	if ((pid = ::tcgetsid(fd)) == -1)
		throw Error(errno);
	return pid;
}

/**
	Calls tcsendbreak.
*/
void Terminal::tcsendbreak(int duration)
{
	if (::tcsendbreak(fd, duration) == -1)
		throw Error(errno);
}

/**
	Calls tcsetattr.
*/
void Terminal::tcsetattr(int actions, const Termios& t)
{
	if (::tcsetattr(fd, actions, &t) == -1)
		throw Error(errno);
}

/**
	Calls tcsetpgrp.
*/
void Terminal::tcsetpgrp(pid_t pgid)
{
	if (::tcsetpgrp(fd, pgid) == -1)
		throw Error(errno);
}

/**
	Calls ttyname_r (ttyname not supported). Could be redesigned to use the path in the
	superclass, but this function is too rarely used for that to be worthwhile.
*/
void Terminal::ttyname(char *buf, size_t bufsize)
{
	int r;

	if ((r = ::ttyname_r(fd, buf, bufsize)) > 0)
		throw Error(r);
}
