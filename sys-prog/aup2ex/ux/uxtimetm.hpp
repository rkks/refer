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
#ifndef _UXTIMETM_HPP_
#define _UXTIMETM_HPP_

#include <wchar.h>

namespace Ux {

/**
	\ingroup Ux
	Don't use this class -- use TimeSec instead.
*/
class Timet : public Base {
protected:
	time_t tt;

public:
	Timet()
		: tt(0)
		{ }
	operator time_t() const
		{ return tt; }
	void set(time_t t)
		{ tt = t; }
	void set(void)
		{ tt = time(); }
	double difftime(time_t time0);
	static time_t time(void);
};

class Timetm;

/**
	\ingroup Ux
	Don't use this class -- use TimeString instead.
*/
class Timestr : public Base {
friend Timetm;

protected:
	char timestr[26];

public:
	Timestr()
		{
			clear();
		}
	void clear(void)
		{
			timestr[0] = '\0';
		}
	operator const char*() const
		{ return timestr; }
	const char *ctime(const time_t *t = NULL, bool keep_nl = true);
	/**
		Version of ctime that takes a time_t instead of a pointer to one.
	**/
	const char *ctime(time_t t, bool keep_nl = true)
		{ return ctime(&t, keep_nl); }
};

/**
	\ingroup Ux
	Don't use this class -- use TimeParts instead.
*/
class Timetm : public tm, public Base {
protected:
	Timestr str;

public:
	/**
		Initializes to local time (at epoch by default).
	*/
	Timetm(time_t t = 0)
		{
			localtime(t);
		}
	operator const char*()
		{ return asctime(false); }
	void set(const struct tm& t);
	time_t mktime(void);
	const char *asctime(bool keep_nl = true);
	void getdate(const char *s);
	void gmtime(const time_t *t = NULL);
	/**
		Version of gmtime that takes a time_t instead of a pointer to one.
	**/
	void gmtime(time_t t)
		{ return gmtime(&t); }
	void localtime(const time_t *t = NULL);
	/**
		Version of localtime that takes a time_t instead of a pointer to one.
	**/
	void localtime(time_t t)
		{ return localtime(&t); }
	char *strptime(const char *s, const char *format);
	size_t strftime(char *buf, size_t bufsize, const char *format);
	size_t wcsftime(wchar_t *buf, size_t bufsize, const wchar_t *format);
};

/**
	\ingroup Ux
	Don't use this class -- use TimeMsec instead.
*/
class Timeval : public timeval, public Base {
public:
	void gettimeofday(void);
};

} // namespace

#endif // _UXTIMETM_HPP_
