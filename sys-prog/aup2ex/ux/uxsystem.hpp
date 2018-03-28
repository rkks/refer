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
#ifndef _UXSYSTEM_HPP_
#define _UXSYSTEM_HPP_

#include <sys/utsname.h>
#include <pwd.h>
#include <grp.h>

namespace Ux {

/**
	\ingroup Ux
*/
class System : public Base {
protected:

public:
	System()
		{ }

	static size_t			confstr(int name, char *buf, size_t len, long default_val = -1);
	static long				sysconf(int name, long default_val = -1);
	static void				uname(struct utsname *name);
	static struct group *	getgrgid(gid_t gid);
	static void				gethostname(char *name, size_t namelen);
	/**
		Calls gethostid.
	*/
	static long				gethostid(void)
								{ return ::gethostid(); }
	static char *					getlogin(void);
	static struct passwd *	getpwuid(uid_t uid);
};

} // namespace

#endif // _UXSYSTEM_HPP_
