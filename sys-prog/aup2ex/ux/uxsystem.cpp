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
	Calls confstr.
*/
/* static */ size_t System::confstr(int name, char *buf, size_t len, long default_val)
{
	size_t r;

	errno = 0;
	r = ::confstr(name, buf, len);
	if (r == 0) {
		if (errno != 0)
			throw Error(errno);
		else
			r = default_val;
	}
	return r;
}

/**
	Calls gethostname.
*/
/* static */ void System::gethostname(char *name, size_t namelen)
{
	if (::gethostname(name, namelen) == -1)
		throw Error(errno);
}

/**
	Calls sysconf.
*/
/* static */ long System::sysconf(int name, long default_val)
{
	long r;

	errno = 0;
	r = ::sysconf(name);
	if (r == -1) {
		if (errno != 0)
			throw Error(errno);
		else
			r = default_val;
	}
	return r;
}

/**
	Calls uname.
*/
/* static */ void System::uname(struct utsname *name)
{
	if (::uname(name) == -1)
		throw Error(errno);
}

/**
	Calls getgrgid.
*/
/* static */ struct group *System::getgrgid(gid_t gid)
{
	struct group *p;

	if ((p = ::getgrgid(gid)) == NULL)
		throw Error(errno);
	return p;
}

/**
	Calls getlogin.
*/
/* static */ char *System::getlogin(void)
{
	char *p;

	if ((p = ::getlogin()) == NULL)
		throw Error(errno);
	return p;
}

/**
	Calls getpwuid.
*/
/* static */ struct passwd *System::getpwuid(uid_t uid)
{
	struct passwd *p;

	if ((p = ::getpwuid(uid)) == NULL)
		throw Error(errno);
	return p;
}
