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

using namespace Ux;

/**
	Calls mktime.
*/
TimeSec TimeParts::get_secs(void)
{
	time_t t;

	errno = 0;
	if ((t = ::mktime(this)) == -1) {
		if (errno == 0)
			errno = EINVAL; // SUS defines no errno values -- make one up
		throw Error(errno);
	}
	return TimeSec(t);
}

/**
	Calls asctime_t; optionally removes newline.
*/
const char *TimeParts::get_string(bool keep_nl)
{
	errno = 0;
	if (::asctime_r(this, str.timestr) == NULL) {
		if (errno == 0)
			throw Error(EINVAL); // SUS defines no errno values -- make one up
		throw Error(errno);
	}
	if (!keep_nl)
		str.timestr[strlen(str.timestr) - 1] = '\0';
	return str;
}

/**
	Calls getdate.
*/
void TimeParts::set(const char *s)
{
	struct tm *t;

	if ((t = ::getdate(s)) == NULL)
		throw Error(getdate_err, EC_GETDATE);
	set(*t);
}
#if 0

void TimeParts::gmtime(const time_t *t)
{
	time_t tx;

	if (t == NULL) {
		tx = TimeSec::time();
		t = &tx;
	}
	if (::gmtime_r(t, this) == NULL)
		throw Error(errno);
}

void TimeParts::localtime(const time_t *t)
{
	time_t tx;

	if (t == NULL) {
		tx = TimeSec::time();
		t = &tx;
	}
	if (::localtime_r(t, this) == NULL)
		throw Error(errno);
}
#endif

/**
	Sets parts according to local time (calling localtime_r) or GMT (calling gmtime_r).
*/
void TimeParts::set(time_t t, zone z)
{
	if (z == zone_local) {
		if (::localtime_r(&t, this) == NULL)
			throw Error(errno);
	}
	else {
		if (::gmtime_r(&t, this) == NULL)
			throw Error(errno);
	}
		
}

/**
	Sets from another TimeParts or struct tm.
*/
void TimeParts::set(const struct tm& t)
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
	str.set();
}

/**
	Sets parts from string according to format (calls strptime).
*/
char *TimeParts::set_fmt(const char *s, const char *format)
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

/**
	Gets string from parts according to format (calls strftime).
*/
size_t TimeParts::get_fmt(char *buf, size_t bufsize, const char *format)
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

/**
	Gets string from parts according to format; wide-character version (calls wstrftime).
*/
size_t TimeParts::get_fmtw(wchar_t *buf, size_t bufsize, const wchar_t *format)
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

/**
	Sets string from TimeSec or time_t (calls ctime). Optionally removes newline.
*/
void TimeString::set(time_t t, bool keep_nl)
{
	if (::ctime_r(&t, timestr) == NULL)
		throw Error(errno);
	if (!keep_nl)
		timestr[strlen(timestr) - 1] = '\0';
}

/**
	Constructor to set time to epoch or now.
*/
TimeSec::TimeSec(when w)
{
	if (w == epoch)
		tt = 0;
	else
		tt = ::time(NULL);
}

/**
	Subtracts TimeSec or time_t (calls difftime).
*/
double TimeSec::diff(time_t time0)
{
	return ::difftime(tt, time0);
}

/**
	Gets current or epoch time in msec (calls gettimeofday).
*/
TimeMsec::TimeMsec(when w)
{
	if (w == epoch) {
		tv_sec = 0;
		tv_usec = 0;
	}
	else {
		errno = 0;
		if (::gettimeofday(this, NULL) != 0) {
			if (errno == 0)
				errno = EINVAL; // SUS defines no errno values -- make one up
			throw Error(errno);
		}
	}
}

/**
	Gets TimeMsec as string.
*/
const char *TimeMsec::get_string(char *buf, size_t bufsize)
{
	snprintf(buf, bufsize, "%lld.%.6ld", (long long)tv_sec, (long)tv_usec);
	return buf;
}

/**
	Gets current or epoch time in nsec (calls gettimeofday).
*/
TimeNsec::TimeNsec(when w)
{
	if (w == epoch) {
		tv_sec = 0;
		tv_nsec = 0;
	}
	else
		Clock().gettime(*this);
}

/**
	Gets TimeNsec as string.
*/
const char *TimeNsec::get_string(char *buf, size_t bufsize)
{
	snprintf(buf, bufsize, "%lld.%.9ld", (long long)tv_sec, (long)tv_nsec);
	return buf;
}

/**
	Stream operator for timeval.
*/
std::ostream& Ux::operator<<(std::ostream& s, const timeval& t)
{
	s << t.tv_sec << "." << setw(6) << setfill('0') << t.tv_usec;
	return s;
}

/**
	Stream operator for timespec.
*/
std::ostream& Ux::operator<<(std::ostream& s, const timespec& t)
{
	s << t.tv_sec << "." << setw(9) << setfill('0') << t.tv_nsec;
	return s;
}

/**
	Calls clock_gettime.
*/
void Clock::gettime(TimeNsec& t)
{
	if (::clock_gettime(clock_id, &t) == -1)
		throw Error(errno);
}

/**
	Calls clock_getres.
*/
void Clock::getres(TimeNsec& res)
{
	if (::clock_getres(clock_id, &res) == -1)
		throw Error(errno);
}

/**
	Calls clock_settime.
*/
void Clock::settime(const TimeNsec& t)
{
	if (::clock_settime(clock_id, &t) == -1)
		throw Error(errno);
}

/**
	Calls clock_nanosleep; returns remaining time.
*/
void Clock::nanosleep(int flags, const TimeNsec& nsecs, const TimeNsec& remain)
{
#if _POSIX_CLOCK_SELECTION > 0
	int r;
	
	if ((r = ::clock_nanosleep(clock_id, flags, &nsecs, &remain)) > 0)
		throw Error(r);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls clock_nanosleep; does not return remaining time.
*/
void Clock::nanosleep(int flags, const TimeNsec& nsecs)
{
#if _POSIX_CLOCK_SELECTION > 0
	int r;
	
	if ((r = ::clock_nanosleep(clock_id, flags, &nsecs, NULL)) > 0)
		throw Error(r);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls nanosleep; returns remaining time.
*/
/* static */void Clock::nanosleep(const TimeNsec& nsecs, TimeNsec& remain)
{
	int r;
	
	if ((r = ::nanosleep(&nsecs, &remain)) > 0)
		throw Error(r);
}

/**
	Calls nanosleep; does not return remaining time.
*/
/* static */void Clock::nanosleep(const TimeNsec& nsecs)
{
	int r;
	
	if ((r = ::nanosleep(&nsecs, NULL)) > 0)
		throw Error(r);
}

/**
	Calls clock_getcpuclockid.
*/
/* static */void Clock::getcpuclockid(pid_t pid, Clock& clock)
{
#if _POSIX_CPUTIME  > 0
	int r;
	
	if ((r = ::clock_getcpuclockid(pid, &clock.clock_id)) > 0)
		throw Error(r);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls ::usleep.
*/
/* static */ void Clock::usleep(useconds_t usecs)
{
	if (::usleep(usecs) == -1)
		throw Error(errno);
}

/**
	Calls ::timer_create.
*/
void RealtimeTimer::create(const Clock& clock, struct sigevent *sig)
{
	if (::timer_create(clock, sig, &timer_id) == -1)
		throw Error(errno);
}

/**
	Calls ::timer_delete.
*/
void RealtimeTimer::del(void)
{
	if (::timer_delete(timer_id) == -1)
		throw Error(errno);
}

/**
	Calls ::timer_gettime.
*/
void RealtimeTimer::get(struct itimerspec& val)
{
	if (::timer_gettime(timer_id, &val) == -1)
		throw Error(errno);
}

/**
	Calls ::timer_settime.
*/
void RealtimeTimer::set(int flags, const struct itimerspec& val, struct itimerspec& oval)
{
	if (::timer_settime(timer_id, flags, &val, &oval) == -1)
		throw Error(errno);
}

/**
	Calls ::getitimer.
*/
void IntervalTimer::get(struct itimerval& val)
{
	if (::getitimer(which_timer, &val) == -1)
		throw Error(errno);
}

/**
	Calls ::setitimer.
*/
void IntervalTimer::set(const struct itimerval& val, struct itimerval& oval)
{
	if (::setitimer(which_timer, &val, &oval) == -1)
		throw Error(errno);
}

#if 0
	//#include <sys/time.h>
void TimeMsec::now(void)
{
	errno = 0;
	if (::gettimeofday(this, NULL) != 0) {
		if (errno == 0)
			errno = EINVAL; // SUS defines no errno values -- make one up
		throw Error(errno);
	}
}

#endif
