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
#include <iomanip>
#include <sys/time.h>

using namespace Ux;

time_t Timetm::mktime(void)
{
	time_t t;

	errno = 0;
	if ((t = ::mktime(this)) == -1) {
		if (errno == 0)
			errno = EINVAL; // SUS defines no errno values -- make one up
		throw Error(errno);
	}
	return t;
}

const char *Timetm::asctime(bool keep_nl)
{
	if (::asctime_r(this, str.timestr) == NULL)
		throw Error(EINVAL); // SUS defines no errno values -- make one up
	if (!keep_nl)
		str.timestr[strlen(str.timestr) - 1] = '\0';
	return str;
}

void Timetm::getdate(const char *s)
{
	struct tm *t;

	if ((t = ::getdate(s)) == NULL)
		throw Error(getdate_err, EC_GETDATE);
	set(*t);
}

void Timetm::gmtime(const time_t *t)
{
	time_t tx;

	if (t == NULL) {
		tx = Timet::time();
		t = &tx;
	}
	if (::gmtime_r(t, this) == NULL)
		throw Error(errno);
}

void Timetm::localtime(const time_t *t)
{
	time_t tx;

	if (t == NULL) {
		tx = Timet::time();
		t = &tx;
	}
	if (::localtime_r(t, this) == NULL)
		throw Error(errno);
}

void Timetm::set(const struct tm& t)
{
	tm_sec = t.tm_sec;
	tm_min = t.tm_min;
	tm_hour = t.tm_hour;
	tm_mday = t.tm_mday;
	tm_mon = t.tm_mon;
	tm_year = t.tm_year;
	tm_wday = t.tm_wday;
	tm_yday = t.tm_yday;
	tm_isdst = t.tm_isdst;
	str.clear();
}

char *Timetm::strptime(const char *s, const char *format)
{
	char *p;
	
	errno = 0;
	if ((p = ::strptime(s, format, this)) == NULL) {
		if (errno == 0)
			errno = EINVAL; // SUS defines no errno values -- make one up
		throw Error(errno);
	}
	return p;
}

size_t Timetm::strftime(char *buf, size_t bufsize, const char *format)
{
	size_t n;
	
	errno = 0;
	if ((n = ::strftime(buf, bufsize, format, this)) == 0) {
		if (errno == 0)
			errno = EINVAL; // SUS defines no errno values -- make one up
		throw Error(errno);
	}
	return n;
}

size_t Timetm::wcsftime(wchar_t *buf, size_t bufsize, const wchar_t *format)
{
	size_t n;
	
	errno = 0;
	if ((n = ::wcsftime(buf, bufsize, format, this)) == 0) {
		if (errno == 0)
			errno = EINVAL; // SUS defines no errno values -- make one up
		throw Error(errno);
	}
	return n;
}

const char *Timestr::ctime(const time_t *t, bool keep_nl)
{
	time_t tx;

	if (t == NULL) {
		tx = Timet::time();
		t = &tx;
	}
	if (::ctime_r(t, timestr) == NULL)
		throw Error(errno);
	if (!keep_nl)
		timestr[strlen(timestr) - 1] = '\0';
	return timestr;
}

/* static */ time_t Timet::time(void)
{
	time_t t;

	errno = 0;
	if ((t = ::time(NULL)) == -1) {
		if (errno == 0)
			errno = EINVAL; // SUS defines no errno values -- make one up
		throw Error(errno);
	}
	return t;
}

double Timet::difftime(time_t time0)
{
	return ::difftime(tt, time0);
}

	//#include <sys/time.h>
void Timeval::gettimeofday(void)
{
	errno = 0;
	if (::gettimeofday(this, NULL) != 0) {
		if (errno == 0)
			errno = EINVAL; // SUS defines no errno values -- make one up
		throw Error(errno);
	}
}
