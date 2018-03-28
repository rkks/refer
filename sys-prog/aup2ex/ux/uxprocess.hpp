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
#ifndef _UXPROCESS_HPP_
#define _UXPROCESS_HPP_

#include "ux.hpp"

#include <sys/uio.h>
#include <utime.h>
#include <sys/times.h>

#ifndef _XOPEN_UNIX
	typedef int idtype_t;
	typedef int id_t;
	typedef unsigned int useconds_t;
#endif

namespace Ux {

/**
	\ingroup Ux
	Class for process-related functions. Most operate only on the process from which
	they're called, so are static because it would be misleading to suggest that
	they can operate on an arbitrary Process. (That is, the underlying function doesn't
	take a pid_t argument.)
*/
class Process : public Base {
protected:
	pid_t pid;

public:
	Process(pid_t id = -1)
		: pid(id == -1 ? ::getpid() : id)
		{ }
	void set(pid_t id)
		{ pid = id; }
	operator pid_t()
		{ return pid; }

	/**
		Calls ::abort.
	*/
	static void 	abort(void)
						{ ::abort(); }
	static void		atexit(void (*fcn)(void));
	static void 	chdir(const char *path);
	static void 	chdir(int fd);
	static void 	chroot(const char *path);
	static clock_t	clock(void);
	static void		execlp(const char *file, const char *arg0, ...);
// execvpe member functions not thread safe because of setting of environ
	static void		execvpe(const char *file, char *const argv[], char *const *envv = NULL);
	/**
		Calls ::_exit.
	*/
	static void		_exit(int status = EXIT_SUCCESS)
						{ ::_exit(status); }
	/**
		Calls ::exit.
	*/
	static void		exit(int status = EXIT_SUCCESS)
						{ ::exit(status); }
	static Process	fork(void);
	static void 	getcwd(Dir& d);
	/**
		Calls ::getegid.
	*/
	static gid_t	getegid(void)
						{ return ::getegid(); }
	static char*	getenv(const char *var);
	/**
		Calls ::geteuid.
	*/
	static uid_t	geteuid(void)
						{ return ::geteuid(); }
	/**
		Calls ::getgid.
	*/
	static gid_t	getgid(void)
						{ return ::getgid(); }
	/**
		Calls ::getpid.
	*/
	static pid_t	getpid(void)
						{ return ::getpid(); }
	static pid_t	getpgid(pid_t pid);
	/**
		Calls ::getppid.
	*/
	static pid_t	getppid(void)
						{ return ::getppid(); }
	static void 	getrlimit(int resource, struct rlimit *rlp);
	static void 	getrusage(int who, struct rusage *r_usage);
	pid_t			getsid(void);
	/**
		Calls ::getuid.
	*/
	static uid_t	getuid(void)
						{ return ::getuid(); }
	static void		kill(pid_t pid, int signum);
	/**
		Calls ::kill.
	*/
	void			kill(int signum)
						{ return kill(pid, signum); }
	static void 	nice(int incr);
	static void		pause(void);
	static void		putenv(char *string);
	static void		setegid(gid_t gid);
	static void		setenv(const char *var, const char *val, int overwrite);
	static void		seteuid(uid_t uid);
	static void		setgid(gid_t gid);
	static void		setpgid(pid_t pid, pid_t pgid);
	static void 	setrlimit(int resource, const struct rlimit *rlp);
	static pid_t	setsid(void);
	static void		setuid(uid_t uid);
	static void		sigaction(int signum, const struct sigaction *act, struct sigaction *oact = NULL);
	static void		sigaltstack(const stack_t *stack, stack_t *ostack = NULL);
	static void		siginterrupt(int signum, int on = true);
	static void		sigprocmask(int how, const sigset_t *set, sigset_t *oset = NULL);
	void			sigqueue(int signum, const union sigval value);
	static void		sigsuspend(const sigset_t *sigmask);
	static int		sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *ts);
	static void		sigwait(const sigset_t *set, int *signum);
	static int		sigwaitinfo(const sigset_t *set, siginfo_t *info = NULL);

	static int		system(const char *command);
	static clock_t	times(struct tms *buffer);
	/**
		Calls ::umask.
	*/
	static mode_t	umask(mode_t cmask)
						{ return ::umask(cmask); }
	static void		unsetenv(const char *var);
	/**
		Calls ::wait.
	*/
	static pid_t 	wait(ExitStatus *statusp = NULL)
						{ return Process::waitpid(-1, statusp); }
	static void		waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options = 0);
	static pid_t 	waitpid(pid_t pid, ExitStatus *statusp = NULL, int options = 0);
	/**
		Calls ::waitpid. The static version takes a pid_t argument in case you want to wait
		for other than the calling process.
	*/
	pid_t 			waitpid(ExitStatus *statusp = NULL, int options = 0)
						{ return Process::waitpid(pid, statusp, options); }
};

} // namespace

#endif // _UXPROCESS_HPP_
