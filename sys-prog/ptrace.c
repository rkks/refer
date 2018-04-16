/*
 * ptrace a child - baby demo example - i386 only
 *
 * call: "ptrace command args" to trace command
 *       "ptrace -p N" to trace process with pid N
 */
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <linux/user.h>		/* ORIG_EAX */
#include <linux/unistd.h>	/* __NR_exit */
#include <linux/ptrace.h>	/* EAX */

#define SIZE(a)	(sizeof(a)/sizeof((a)[0]))

/*
 * syscall names for i386 under 2.5.51, taken from <asm/unistd.h>
 */
char *(syscall_names[256]) = {
"exit", "fork", "read", "write", "open", "close", "waitpid", "creat",
"link", "unlink", "execve", "chdir", "time", "mknod", "chmod",
"lchown", "break", "oldstat", "lseek", "getpid", "mount", "umount",
"setuid", "getuid", "stime", "ptrace", "alarm", "oldfstat", "pause",
"utime", "stty", "gtty", "access", "nice", "ftime", "sync", "kill",
"rename", "mkdir", "rmdir", "dup", "pipe", "times", "prof", "brk",
"setgid", "getgid", "signal", "geteuid", "getegid", "acct", "umount2",
"lock", "ioctl", "fcntl", "mpx", "setpgid", "ulimit", "oldolduname",
"umask", "chroot", "ustat", "dup2", "getppid", "getpgrp", "setsid",
"sigaction", "sgetmask", "ssetmask", "setreuid", "setregid",
"sigsuspend", "sigpending", "sethostname", "setrlimit", "getrlimit",
"getrusage", "gettimeofday", "settimeofday", "getgroups", "setgroups",
"select", "symlink", "oldlstat", "readlink", "uselib", "swapon",
"reboot", "readdir", "mmap", "munmap", "truncate", "ftruncate",
"fchmod", "fchown", "getpriority", "setpriority", "profil", "statfs",
"fstatfs", "ioperm", "socketcall", "syslog", "setitimer", "getitimer",
"stat", "lstat", "fstat", "olduname", "iopl", "vhangup", "idle",
"vm86old", "wait4", "swapoff", "sysinfo", "ipc", "fsync", "sigreturn",
"clone", "setdomainname", "uname", "modify_ldt", "adjtimex",
"mprotect", "sigprocmask", "create_module", "init_module",
"delete_module", "get_kernel_syms", "quotactl", "getpgid", "fchdir",
"bdflush", "sysfs", "personality", "afs_syscall", "setfsuid",
"setfsgid", "_llseek", "getdents", "_newselect", "flock", "msync",
"readv", "writev", "getsid", "fdatasync", "_sysctl", "mlock",
"munlock", "mlockall", "munlockall", "sched_setparam",
"sched_getparam", "sched_setscheduler", "sched_getscheduler",
"sched_yield", "sched_get_priority_max", "sched_get_priority_min",
"sched_rr_get_interval", "nanosleep", "mremap", "setresuid",
"getresuid", "vm86", "query_module", "poll", "nfsservctl",
"setresgid", "getresgid", "prctl","rt_sigreturn","rt_sigaction",
"rt_sigprocmask", "rt_sigpending", "rt_sigtimedwait",
"rt_sigqueueinfo", "rt_sigsuspend", "pread", "pwrite", "chown",
"getcwd", "capget", "capset", "sigaltstack", "sendfile", "getpmsg",
"putpmsg", "vfork", "ugetrlimit", "mmap2", "truncate64",
"ftruncate64", "stat64", "lstat64", "fstat64", "lchown32", "getuid32",
"getgid32", "geteuid32", "getegid32", "setreuid32", "setregid32",
"getgroups32", "setgroups32", "fchown32", "setresuid32",
"getresuid32", "setresgid32", "getresgid32", "chown32", "setuid32",
"setgid32", "setfsuid32", "setfsgid32", "pivot_root", "mincore",
"madvise", "getdents64", "fcntl64", 0, "security", "gettid",
"readahead", "setxattr", "lsetxattr", "fsetxattr", "getxattr",
"lgetxattr", "fgetxattr", "listxattr", "llistxattr", "flistxattr",
"removexattr", "lremovexattr", "fremovexattr", "tkill", "sendfile64",
"futex", "sched_setaffinity", "sched_getaffinity", "set_thread_area",
"get_thread_area", "io_setup", "io_destroy", "io_getevents", "io_submit",
"io_cancel", "fadvise64", 0, "exit_group", "lookup_dcookie"
};

void my_ptrace_void(int request, pid_t pid, void *addr, void *data) {
	int i = ptrace(request, pid, addr, data);
	if (i) {
		perror("ptrace");
		exit(1);
	}
}

/*
 * Since -1 may be valid data, we have to check errno.
 */
int my_ptrace_read(int request, pid_t pid, void *addr, void *data) {
	int i;

	errno = 0;
	i = ptrace(request, pid, addr, data);
	if (i == -1 && errno) {
		perror("ptrace");
		exit(1);
	}
	return i;
}

pid_t pid;	/* the traced program */

/* detach from traced program when interrupted */
void interrupt(int dummy) {
	ptrace(PTRACE_DETACH, pid, 0, 0);
	exit(-1);
}

int got_sig = 0;

void sigusr1(int dummy) {
	got_sig = 1;
}

void my_kill(pid_t pid, int sig) {
	int i = kill(pid, sig);
	if (i) {
		perror("kill");
		exit(1);
	}
}

/*
 * A child stopped at a syscall has status as if it received SIGTRAP.
 * In order to distinguish between SIGTRAP and syscall, some kernel
 * versions have the PTRACE_O_TRACESYSGOOD option, that sets an extra
 * bit 0x80 in the syscall case.
 */
#define SIGSYSTRAP	(SIGTRAP | sysgood_bit)

int sysgood_bit = 0;

void set_sysgood(pid_t p) {
#ifdef PTRACE_O_TRACESYSGOOD
	int i = ptrace(PTRACE_SETOPTIONS, p, 0, (void*) PTRACE_O_TRACESYSGOOD);
	if (i == 0)
		sysgood_bit = 0x80;
	else
		perror("PTRACE_O_TRACESYSGOOD");
#endif
}

#define EXPECT_EXITED	1
#define EXPECT_SIGNALED	2
#define	EXPECT_STOPPED	4

void my_wait(pid_t p, int report, int stopsig) {
	int status;
	pid_t pw = wait(&status);

	if (pw == (pid_t) -1) {
		perror("wait");
		exit(1);
	}

	/*
	 * Report only unexpected things.
	 *
	 * The conditions WIFEXITED, WIFSIGNALED, WIFSTOPPED
	 * are mutually exclusive:
	 * WIFEXITED:  (status & 0x7f) == 0, WEXITSTATUS: top 8 bits
	 * and now WCOREDUMP:  (status & 0x80) != 0
	 * WIFSTOPPED: (status & 0xff) == 0x7f, WSTOPSIG: top 8 bits
	 * WIFSIGNALED: all other cases, (status & 0x7f) is signal.
	 */
	if (WIFEXITED(status) && !(report & EXPECT_EXITED))
		fprintf(stderr, "child exited%s with status %d\n",
			WCOREDUMP(status) ? " and dumped core" : "",
			WEXITSTATUS(status));
	if (WIFSTOPPED(status) && !(report & EXPECT_STOPPED))
		fprintf(stderr, "child stopped by signal %d\n",
			WSTOPSIG(status));
	if (WIFSIGNALED(status) && !(report & EXPECT_SIGNALED))
		fprintf(stderr, "child signalled by signal %d\n",
			WTERMSIG(status));

	if (WIFSTOPPED(status) && WSTOPSIG(status) != stopsig) {
		/* a different signal - send it on and wait */
		fprintf(stderr, "Waited for signal %d, got %d\n",
			stopsig, WSTOPSIG(status));
		if ((WSTOPSIG(status) & 0x7f) == (stopsig & 0x7f))
			return;
		my_ptrace_void(PTRACE_SYSCALL, p, 0, (void*) WSTOPSIG(status));
		return my_wait(p, report, stopsig);
	}

	if ((report & EXPECT_STOPPED) && !WIFSTOPPED(status)) {
		fprintf(stderr, "Not stopped?\n");
		exit(1);
	}
}

/*
 * print value when changed
 */
void outlonghex(unsigned long old, unsigned long new) {
	if (old == new)
		fprintf(stderr, "         ");
	else
		fprintf(stderr, " %08lx", new);
}

int
main(int argc, char **argv, char **envp){
	pid_t p0, p;

	if (argc <= 1) {
		fprintf(stderr, "Usage: %s command args -or- %s -p pid\n",
			argv[0], argv[0]);
		exit(1);
	}

	if (argc >= 3 && !strcmp(argv[1], "-p")) {
		pid = p = atoi(argv[2]);

		signal(SIGINT, interrupt);

		/*
		 * attach to specified process
		 */
		my_ptrace_void(PTRACE_ATTACH, p, 0, 0);
		my_wait(p, EXPECT_STOPPED, SIGSTOP);
		set_sysgood(p);

		/*
		 * we stopped the program in the middle of what it was doing
		 * continue it, and make it stop at the next syscall
		 */
		my_ptrace_void(PTRACE_SYSCALL, p, 0, 0);
	} else {
		void (*oldsig)(int);

		/*
		 * fork off a child that executes the specified command
		 */

		/*
		 * The parent process will send a signal to the child
		 * and do a wait() to wait until the child stops.
		 * If the signal arrives before the child has said
		 * PTRACE_TRACEME, then maybe the child is killed, or
		 * maybe the signal is ignored and we wait forever, or
		 * maybe the child is stopped but we are not tracing.
		 * So, let us arrange for the child to signal the parent
		 * when it has done the PTRACE_TRACEME.
		 */

		/* prepare both parent and child for signal */
		oldsig = signal(SIGUSR1, sigusr1);
		if (oldsig == SIG_ERR) {
			perror("signal");
			exit(1);
		}

		/* child needs parent pid */
		p0 = getpid();

		p = fork();
		if (p == (pid_t) -1) {
			perror("fork");
			_exit(1);
		}

		if (p == 0) {	/* child */
			my_ptrace_void(PTRACE_TRACEME, 0, 0, 0);

			/* tell parent that we are ready */
			my_kill(p0, SIGUSR1);

			/* wait for parent to start tracing us */
			while (!got_sig) ;

			/*
			 * the first thing the parent will see is
			 *  119: sigreturn - the return from the signal handler
			 */

			/* exec the given process */
			argv[argc] = 0;
			execve(argv[1], argv+1, envp);
			exit(1);
		}

		/* wait for child to get ready */
		while (!got_sig) ;

		/*
		 * tell child that we got the signal
		 * this kill() will stop the child
		 */
		my_kill(p, SIGUSR1);
		my_wait(p, EXPECT_STOPPED, SIGUSR1);
		set_sysgood(p);

		my_ptrace_void(PTRACE_SYSCALL, p, 0, (void *) SIGUSR1);
	}

	/*
	 * trace the victim's syscalls
	 */
	while (1) {
		int syscall;
		struct user_regs_struct u_in, u_out;

		my_wait(p, EXPECT_STOPPED, SIGSYSTRAP);

		my_ptrace_void(PTRACE_GETREGS, p, 0, &u_in);
		syscall = u_in.orig_eax;

		fprintf(stderr, "SYSCALL %3d:", syscall);
		outlonghex(-38, u_in.eax);	/* seems constant */
		fprintf(stderr, " %08lx %08lx %08lx",
			u_in.ebx, u_in.ecx, u_in.edx);
		if (syscall-1 >= 0 && syscall-1 < SIZE(syscall_names) &&
		    syscall_names[syscall-1])
			fprintf(stderr, " /%s", syscall_names[syscall-1]);
		fprintf(stderr, "\n");

		if (syscall == __NR_execve) {
			long *regs = 0; /* relative address 0 in user area */
			long eax;

			my_ptrace_void(PTRACE_SYSCALL, p, 0, 0);
			my_wait(p, EXPECT_STOPPED, SIGSYSTRAP);

			/*
			 * For a successful execve we get one more trap
			 * But was this call successful?
			 */
			eax = my_ptrace_read(PTRACE_PEEKUSER, p, &regs[EAX],0);
			if (eax == 0) {
				fprintf(stderr, "SYSCALL execve, once more\n");

				/* the syscall return - no "good" bit */
				my_ptrace_void(PTRACE_SYSCALL, p, 0, 0);
				my_wait(p, EXPECT_STOPPED, SIGTRAP);
			}
		} else {
			/* wait for syscall return */
			my_ptrace_void(PTRACE_SYSCALL, p, 0, 0);
			if (syscall == __NR_exit ||
			    syscall == __NR_exit_group) {
				my_wait(p, EXPECT_EXITED, 0);
				exit(0);
			}
			my_wait(p, EXPECT_STOPPED, SIGSYSTRAP);
		}

		my_ptrace_void(PTRACE_GETREGS, p, 0, &u_out);
		fprintf(stderr, " RETURN %3d:", syscall);
		outlonghex(u_in.eax, u_out.eax);
		outlonghex(u_in.ebx, u_out.ebx);
		outlonghex(u_in.ecx, u_out.ecx);
		outlonghex(u_in.edx, u_out.edx);
		fprintf(stderr, "\n");

		my_ptrace_void(PTRACE_SYSCALL, p, 0, 0);
	}
	return 0;
}
