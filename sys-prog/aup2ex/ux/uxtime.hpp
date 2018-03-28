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
#ifndef _UXTIME_HPP_
#define _UXTIME_HPP_

#include <wchar.h>
#include <sys/time.h>

namespace Ux {

enum when {epoch, now};

/**
	\ingroup Ux
	Clearer replacement for Timet (time_t).
*/
class TimeSec : public Base {
protected:
	time_t tt;

public:
	TimeSec(time_t t)
		: tt(t)
		{ }
	TimeSec(when w = epoch);
	operator time_t() const
		{ return tt; }
	void set(time_t t)
		{ tt = t; }
	double diff(time_t time0);
};

/**
	\ingroup Ux
	Wrapper class for struct timeval.
*/
class TimeMsec : public timeval, public Base {
public:
	TimeMsec(TimeMsec& t)
		{
			tv_sec = t.tv_sec;
			tv_usec = t.tv_usec;
		}
	TimeMsec(when w = epoch);
	const char *get_string(char *buf, size_t bufsize);
};

/**
	\ingroup Ux
	Wrapper class for struct timespec.
*/
class TimeNsec : public timespec, public Base {
public:
	TimeNsec(TimeNsec& t)
		{
			tv_sec = t.tv_sec;
			tv_nsec = t.tv_nsec;
		}
	TimeNsec(when w = epoch);
	const char *get_string(char *buf, size_t bufsize);
};

//@{
/** \name ostream Operators
Operators to display UNIX times.
*/
std::ostream& operator<<(std::ostream& s, const timeval& t);
std::ostream& operator<<(std::ostream& s, const timespec& t);
//@}

class TimeParts;

/**
	\ingroup Ux
	Clearer replacement for Timestr (char *).
*/
class TimeString : public Base {
friend TimeParts;

protected:
	char timestr[26];

public:
	TimeString(void)
		{
			set();
		}
	TimeString(time_t t, bool keep_nl = true)
		{
			set(t, keep_nl);
		}
	void set(const char *s = "")
		{
			strncpy(timestr, s, sizeof(timestr));
			timestr[sizeof(timestr) - 1] = '\0';
		}
	operator const char*() const
		{ return timestr; }
	void set(time_t t, bool keep_nl = true);
};

/**
	\ingroup Ux
	Clearer replacement for Timetm (struct tm).
*/
class TimeParts : public tm, public Base {
protected:
	TimeString str;

public:
	enum zone {zone_local, zone_utc} ;
	/**
		Initializes to local time (at epoch by default).
	*/
	TimeParts(time_t t = 0)
		{
			set(t);
		}
	operator const char*()
		{ return get_string(false); }
	void set(const struct tm& t);
	TimeSec get_secs(void);
	const char *get_string(bool keep_nl = true);
	void set(const char *s);
	void set(time_t t, zone z = zone_local);
	void set(zone z = zone_local)
		{ return set(TimeSec(now), z); }
	char *set_fmt(const char *s, const char *format);
	size_t get_fmt(char *buf, size_t bufsize, const char *format);
	size_t get_fmtw(wchar_t *buf, size_t bufsize, const wchar_t *format);
};

/**
	\ingroup Ux
	
*/
class Clock : public Base {
protected:
	clockid_t clock_id;

public:
	Clock(clockid_t cid = CLOCK_REALTIME)
		{ clock_id = cid; }
	operator clock_t() const
		{ return clock_id; }
	void 			nanosleep(int flags, const TimeNsec& nsecs);
	void 			nanosleep(int flags, const TimeNsec& nsecs, const TimeNsec& remain);
	static void		nanosleep(const TimeNsec& nsecs, TimeNsec& remain);
	static void		nanosleep(const TimeNsec& nsecs);
	static void		getcpuclockid(pid_t pid, Clock& clock);
	void			getres(TimeNsec& res);
	void			gettime(TimeNsec& t);
	void			settime(const TimeNsec& t);
	/**
		Calls ::sleep.
	*/
	static unsigned	sleep(unsigned secs)
						{ return ::sleep(secs); }
	static void		usleep(useconds_t usecs);
};

/**
	\ingroup Ux
	
*/
class Timer : public Base {
public:
	/**
		Calls ::alarm.
	*/
	static unsigned	alarm(unsigned secs)
						{ return ::alarm(secs); }
};

/**
	\ingroup Ux
	
*/
class RealtimeTimer : public Timer {
protected:
	timer_t timer_id;

public:
	RealtimeTimer(timer_t t = -1)
		: timer_id(t)
		{ }
	void			create(const Clock& clock, struct sigevent *sig = NULL);
	void			del(void);
	void			get(struct itimerspec& val);
	void			set(int flags, const struct itimerspec& val, struct itimerspec& oval);
};

/**
	\ingroup Ux
	
*/
class IntervalTimer : public Timer {
protected:
	int which_timer;

public:
	IntervalTimer(int which = ITIMER_REAL)
		: which_timer(which)
		{ }
	void			get(struct itimerval& val);
	void			set(const struct itimerval& val, struct itimerval& oval);
};

} // namespace

#endif // _UXTIME_HPP_
