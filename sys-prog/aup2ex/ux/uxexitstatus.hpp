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
#ifndef _UXEXITSTATUS_HPP_
#define _UXEXITSTATUS_HPP_

namespace Ux {

/**
	\ingroup Ux
*/
class ExitStatus : public Base {
protected:
	int status;

public:
	ExitStatus(int s = 0)
		: status(s)
		{ }
	void set(int s = 0)
		{ status = s; }
	operator int()
		{ return status; }
};

//@{
/** \name ostream Operators
Operator to display ExitStatus.
*/
std::ostream& operator<<(std::ostream& s, const ExitStatus& s);
//@}

} // namespace

#endif // _UXEXITSTATUS_HPP_
