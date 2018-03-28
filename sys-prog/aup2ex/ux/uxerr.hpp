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
#ifndef _UXERR_HPP_
#define _UXERR_HPP_

#include <exception>
#include <iostream>

#define EC_CAUGHT(e)\
	ec_push(__func__, __FILE__, __LINE__, "EC_CAUGHT",\
	  e.get_code(), e.get_type());

#define EC_EXIT(e)\
	{\
		EC_CAUGHT(e)\
		::exit(EXIT_FAILURE);\
	}

#define EC__EXIT(e)\
	{\
		EC_CAUGHT(e)\
		EC_FLUSH("EC__EXIT")\
		::_exit(EXIT_FAILURE);\
	}

#define EC_CATCH(s)\
	try { s; } catch (Error e) { EC_EXIT(e) }

namespace Ux {

/**
	\ingroup Ux
*/
class Error : public exception {
protected:
	EC_ERRTYPE e_type;
	int e_code;
	char str[50];
public:
	Error(int c = 0, EC_ERRTYPE t = EC_ERRNO)
		: e_type(t), e_code(c)
		{ }
	void set(int c, EC_ERRTYPE t = EC_ERRNO)
		{ e_type = t; e_code = c; }
	EC_ERRTYPE get_type(void) const
		{ return e_type; }
	int get_code(void) const
		{ return e_code; }
	operator int() const
		{ return e_code; }
	operator const char*() const
		{ return what(); }
	virtual const char *what() const throw();
};

//@{
/** \name ostream Operators
Operator to display Error.
*/
std::ostream& operator<<(std::ostream& s, const Error& e);
//@}

} // namespace

#endif // _UXERR_HPP_
