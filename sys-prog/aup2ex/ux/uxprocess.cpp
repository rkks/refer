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
#ifdef SOLARIS
// needed for setegid and seteuid
#define __EXTENSIONS__
#endif

/*
	Need to prevent GNU from using the wrong prototype for functions declared in
	sys/resource.h (see comment there). As sys/wait.h (included by defs.h) brings
	in this include (on GNU systems, anyway), there is a special symbol (AUP2_SKIP_WAIT)
	used in defs.h to suppress that include. Also, we should not have to undef __USE_GNU
	(I think), we seem to have to do that as well.
	
	(It would have been much better if the GNU folks had not tried to improve things!)
*/

#define AUP2_SKIP_WAIT
#include "defs.h"

#ifdef _GNU_SOURCE
#define _GNU_SOURCE_WAS_DEFINED
#undef _GNU_SOURCE
#undef __USE_GNU
#endif
#include <sys/resource.h>
#ifdef _GNU_SOURCE_WAS_DEFINED
#define _GNU_SOURCE
#undef _GNU_SOURCE_WAS_DEFINED
#endif

#include <sys/wait.h> // now it is OK to bring it in
#include "ux.hpp" // and now everything else can come in

using namespace Ux;

/**
	Calls ::atexit.
*/
/* static */ void Process::atexit(void (*fcn)(void))
{
	errno = 0; // SUS doesn't say it's set on error
	if (::atexit(fcn) != 0)
		throw Error(errno);
}

/**
	Calls ::chdir.
*/
/* static */ void Process::chdir(const char *path)
{
	if (::chdir(path) == -1)
		throw Error(errno);
}

/**
	Calls ::fchdir.
*/
/* static */ void Process::chdir(int fd)
{
	if (::fchdir(fd) == -1)
		throw Error(errno);
}

/**
	Calls ::chroot.
*/
/* static */ void Process::chroot(const char *path)
{
	if (::chroot(path) == -1)
		throw Error(errno);
}

/**
	Calls ::clock.
*/
/* static */ clock_t Process::clock(void)
{
	clock_t c;

	errno = 0;
	if ((c = ::clock()) == -1) {
		if (errno == 0)
			throw Error(EINVAL);
		else
			throw Error(errno);
	}
	return c;
}

/**
	Calls ::execvp after building a vector from the arguments. (Not feasible to call ::execlp,
	as there's no portable way to pass it the arguments as a list.)
*/
/* static */ void Process::execlp(const char *file, const char *arg0, ...)
{
	va_list ap;
	int count;

	va_start(ap, arg0);
	for (count = 1; va_arg(ap, char *) != NULL; count++)
		;
	va_end(ap);
	 char **argv = new  char*[count + 1];
	if (argv == NULL)
		throw Error(ENOMEM);
	va_start(ap, arg0);
	argv[0] = (char *)arg0;
	for (count = 1; (argv[count] = va_arg(ap, char *)) != NULL; count++)
		;
	va_end(ap);
	(void)::execvp(file, argv);
	delete [] argv;
	throw Error(errno);
}


/**
	Calls ::execvp, first setting the global environ to the argument if it's non-NULL.
	environ is restored if the call fails. Not thread safe, as setting and re-setting of
	environ isn't protected with a mutex. (It's not that important for an exec call to be
	thread safe, since calling it with more than one thread active is not good practice anyway.)
*/
/* static */ void Process::execvpe(const char *file, char *const argv[], char *const *envv = NULL)
{
	extern char **environ;

	char **environ_save = environ;
	if (envv != NULL)
		environ = (char **)envv;
	(void)execvp(file, argv);
	environ = environ_save;
	throw Error(errno);
}

/**
	Calls ::fork. Returns a Process, which is automatically converted to a pid_t if necessary.
*/
/* static */ Process Process::fork(void)
{
	pid_t rtn_pid;
	
	if ((rtn_pid = ::fork()) == -1)
		throw Error(errno);
	return Process(rtn_pid);
}

/**
	Calls ::getcwd. Dir argument must have space allocated with File::alloc,
	or at least the size and path members set properly.
*/
/* static */ void Process::getcwd(Dir& d)
{
	if (d.get_size() < 0)
		throw Error(ENOMEM);
	// overriding const -- user better have supplied buffer!
	if (::getcwd((char *)(const char *)d, d.get_size()) == NULL)
		throw(errno);
}

/**
	Calls ::getenv. Does not throw error; returns NULL if not found.
*/
/* static */ char* Process::getenv(const char *var)
{
	return ::getenv(var);
}

/**
	Calls ::getpgid.
*/
/* static */ pid_t Process::getpgid(pid_t pid)
{
	pid_t rtn_pid;

	if ((rtn_pid = ::getpgid(pid)) == -1)
		throw Error(errno);
	return rtn_pid;
}

/**
	Calls ::getrlimit.
*/
/* static */ void Process::getrlimit(int resource, struct rlimit *rlp)
{
	if (::getrlimit(resource, rlp) == -1)
		throw Error(errno);
}

/**
	Calls ::getrusage.
*/
/* static */ void Process::getrusage(int who, struct rusage *r_usage)
{
	if (::getrusage(who, r_usage) == -1)
		throw Error(errno);
}

/**
	Calls ::getsid, for this process. getsid(0) (getsid of calling process) not
	implemented and not needed, since a Process is initialized to the calling process
	by default.
*/
pid_t Process::getsid(void)
{
	pid_t rtn_pid;

	if ((rtn_pid = ::getsid(pid)) == -1)
		throw Error(errno);
	return rtn_pid;
}

/**
	Calls ::kill.
*/
/* static */ void Process::kill(pid_t pid, int signum)
{
	if (::kill(pid, signum) == -1)
		throw Error(errno);
}

/**
	Calls ::nice.
*/
/* static */ void Process::nice(int incr)
{
	if (::nice(incr) == -1)
		throw Error(errno);
}

/**
	Calls ::pause.
*/
/* static */ void Process::pause(void)
{
	if (::pause() == -1 && errno != EINTR)
		throw Error(errno);
}

/**
	Calls ::putenv.
*/
/* static */ void Process::putenv(char *string)
{
	if (::putenv(string) != 0)
		throw Error(errno);
}

/**
	Calls ::setegid.
*/
/* static */ void Process::setegid(gid_t gid)
{
	if (::setegid(gid) == -1)
		throw Error(errno);
}

/**
	Calls ::setenv.
*/
/* static */ void Process::setenv(const char *var, const char *val, int overwrite)
{
#if _XOPEN_VERSION >= 600 || defined(FREEBSD) || defined(LINUX)
	if (::setenv(var, val, overwrite) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls ::seteuid.
*/
/* static */ void Process::seteuid(uid_t uid)
{
	if (::seteuid(uid) == -1)
		throw Error(errno);
}

/**
	Calls ::setgid.
*/
/* static */ void Process::setgid(gid_t gid)
{
	if (::setgid(gid) == -1)
		throw Error(errno);
}

/**
	Calls ::setpgid.
*/
/* static */ void Process::setpgid(pid_t pid, pid_t pgid)
{
	if (::setpgid(pid, pgid) == -1)
		throw Error(errno);
}

/**
	Calls ::setrlimit.
*/
/* static */ void Process::setrlimit(int resource, const struct rlimit *rlp)
{
	if (::setrlimit(resource, rlp) == -1)
		throw Error(errno);
}

/**
	Calls ::setsid.
*/
/* static */ pid_t Process::setsid(void)
{
	pid_t rtn_pid;

	if ((rtn_pid = ::setsid()) == -1)
		throw Error(errno);
	return rtn_pid;
}

/**
	Calls ::setuid.
*/
/* static */ void Process::setuid(uid_t uid)
{
	if (::setuid(uid) == -1)
		throw Error(errno);
}

/**
	Calls ::sigaction.
*/
/* static */ void Process::sigaction(int signum, const struct sigaction *act, struct sigaction *oact)
{
	if (::sigaction(signum, act, oact) == -1)
		throw Error(errno);
}

/**
	Calls ::sigaltstack.
*/
/* static */ void Process::sigaltstack(const stack_t *stack, stack_t *ostack)
{
	if (::sigaltstack(stack, ostack) == -1)
		throw Error(errno);
}

/**
	Calls ::siginterrupt.
*/
/* static */ void Process::siginterrupt(int signum, int on)
{
	if (::siginterrupt(signum, on) == -1)
		throw Error(errno);
}

/**
	Calls ::sigprocmask.
*/
/* static */ void Process::sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
	if (::sigprocmask(how, set, oset) == -1)
		throw Error(errno);
}

/**
	Calls ::sigqueue.
*/
void Process::sigqueue(int signum, const union sigval value)
{
#ifdef _POSIX_REALTIME_SIGNALS
	if (::sigqueue(pid, signum, value) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls ::sigsuspend.
*/
/* static */ void Process::sigsuspend(const sigset_t *sigmask)
{
	if (::sigsuspend(sigmask) == -1)
		throw Error(errno);
}
/* Returns -1 on error, always (sets errno) */

/**
	Calls ::sigtimedwait.
*/
/* static */ int Process::sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *ts)
{
#ifdef _POSIX_REALTIME_SIGNALS
	int r;

	if ((r = ::sigtimedwait(set, info, ts)) == -1)
		throw Error(errno);
	return r;
#else
	throw Error(ENOSYS);
#endif
}
/* Returns signal number or -1 on error (sets errno) */

/**
	Calls ::sigwait.
*/
/* static */ void Process::sigwait(const sigset_t *set, int *signum)
{
#if _XOPEN_VERSION >= 500
	if (::sigwait(set, signum) == -1)
		throw Error(errno);
#else
	throw Error(ENOSYS);
#endif
}
/* Returns 0 on success or error number on error */

/**
	Calls ::sigwaitinfo.
*/
/* static */ int Process::sigwaitinfo(const sigset_t *set, siginfo_t *info)
{
#ifdef _POSIX_REALTIME_SIGNALS
	int r;

	if ((r = ::sigwaitinfo(set, info)) == -1)
		throw Error(errno);
	return r;
#else
	throw Error(ENOSYS);
#endif
}
/* Returns signal number or -1 on error (sets errno) */

/**
	Calls ::system.
*/
/* static */ int Process::system(const char *command)
{
	int n;

	if ((n = ::system(command)) == -1)
		throw Error(errno);
	return n;
}

/**
	Calls ::times.
*/
/* static */ clock_t Process::times(struct tms *buffer)
{
	clock_t c;

	if ((c = ::times(buffer)) == -1)
		throw Error(errno);
	return c;
}

/**
	Calls ::unsetenv.
*/
/* static */ void Process::unsetenv(const char *var)
{
#if _XOPEN_VERSION >= 600
	if (::unsetenv(var) == -1)
		throw Error(errno);
#elif defined(FREEBSD) || defined(LINUX)
	::unsetenv(var);
#else
	throw Error(ENOSYS);
#endif
}

/**
	Calls ::waitid if it's supported. Otherwise throws ENOSYS.
*/
/* static */ void Process::waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options)
{
#ifdef _XOPEN_UNIX
	if (::waitid(idtype, id, infop, options) == -1)
		throw Error(errno);
#else
		throw Error(ENOSYS);
#endif
}

/**
	Calls ::waitpid. Use the inline version if you want to operate on the
	Process object's process ID.
*/
/* static */ pid_t Process::waitpid(pid_t pid, ExitStatus *statusp, int options = 0)
{
	pid_t rtn_pid;
	int status;

	if ((rtn_pid = ::waitpid(pid, &status, options)) == -1)
		throw Error(errno);
	if (statusp != NULL)
		statusp->set(status);
	return rtn_pid;
}

