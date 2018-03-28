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
	Stream operator to output status code.
*/
std::ostream& Ux::operator<<(std::ostream& s, const ExitStatus& status)
{
	int n = (ExitStatus&)status; // use variable in macros because of weird FreeBSD definitions

	if (WIFEXITED(n))
		s << "Exit value " << WEXITSTATUS(n);
	else {
		char *desc;
		char *signame = get_macrostr("signal", WTERMSIG(n), &desc);
		if (desc[0] == '?')
			desc = signame;
		if (signame[0] == '?')
			s << "Signal #" << WTERMSIG(n);
		else
			s << desc;
		if (WCOREDUMP(n))
			s << " - core dumped";
		if (WIFSTOPPED(n))
			s << " (stopped)";
#if defined(_XOPEN_UNIX) && !defined(LINUX)
		else if (WIFCONTINUED(n))
			s << " (continued)";
#endif
	}
	return s;
}
