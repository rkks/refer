/* This is an auto-generated file, do not edit */
/* SYSCALLS NAMES */

#ifndef __KSYSCALL_H__
#define __KSYSCALL_H__

#include "/usr/include/machine/resource.h"
#include "/usr/include/openssl/err.h"
#include "/usr/include/sys/eventhandler.h"
#include "/usr/include/sys/syscall.h"
#include "/usr/include/sys/utsname.h"

const char	*gl_syscallname[] =
  {
    "syscall",		/* #0 */
    "exit",		/* #1 */
    "fork",		/* #2 */
    "read",		/* #3 */
    "write",		/* #4 */
    "open",		/* #5 */
    "close",		/* #6 */
    "wait4",		/* #7 */
    "F_ACCEPT",		/* #8 */
    "link",		/* #9 */
    "unlink",		/* #10 */
    "F_FREAD",		/* #11 */
    "chdir",		/* #12 */
    "fchdir",		/* #13 */
    "mknod",		/* #14 */
    "chmod",		/* #15 */
    "chown",		/* #16 */
    "break",		/* #17 */
    NULL,
    NULL,
    "getpid",		/* #20 */
    "mount",		/* #21 */
    "unmount",		/* #22 */
    "setuid",		/* #23 */
    "getuid",		/* #24 */
    "geteuid",		/* #25 */
    "ptrace",		/* #26 */
    "recvmsg",		/* #27 */
    "sendmsg",		/* #28 */
    "recvfrom",		/* #29 */
    "accept",		/* #30 */
    "getpeername",		/* #31 */
    "NMLN",		/* #32 */
    "access",		/* #33 */
    "chflags",		/* #34 */
    "fchflags",		/* #35 */
    "sync",		/* #36 */
    "kill",		/* #37 */
    NULL,
    "getppid",		/* #39 */
    NULL,
    "dup",		/* #41 */
    "pipe",		/* #42 */
    "getegid",		/* #43 */
    "profil",		/* #44 */
    "ktrace",		/* #45 */
    NULL,
    "getgid",		/* #47 */
    NULL,
    "getlogin",		/* #49 */
    "setlogin",		/* #50 */
    "acct",		/* #51 */
    NULL,
    "sigaltstack",		/* #53 */
    "ioctl",		/* #54 */
    "reboot",		/* #55 */
    "revoke",		/* #56 */
    "symlink",		/* #57 */
    "readlink",		/* #58 */
    "execve",		/* #59 */
    "umask",		/* #60 */
    "chroot",		/* #61 */
    NULL,
    NULL,
    NULL,
    "msync",		/* #65 */
    "vfork",		/* #66 */
    NULL,
    NULL,
    "sbrk",		/* #69 */
    "sstk",		/* #70 */
    NULL,
    "vadvise",		/* #72 */
    "munmap",		/* #73 */
    "mprotect",		/* #74 */
    "madvise",		/* #75 */
    NULL,
    NULL,
    "mincore",		/* #78 */
    "getgroups",		/* #79 */
    "setgroups",		/* #80 */
    "getpgrp",		/* #81 */
    "setpgid",		/* #82 */
    "setitimer",		/* #83 */
    NULL,
    "swapon",		/* #85 */
    "getitimer",		/* #86 */
    NULL,
    NULL,
    "getdtablesize",		/* #89 */
    "dup2",		/* #90 */
    NULL,
    "fcntl",		/* #92 */
    "select",		/* #93 */
    NULL,
    "fsync",		/* #95 */
    "setpriority",		/* #96 */
    "socket",		/* #97 */
    "connect",		/* #98 */
    NULL,
    "getpriority",		/* #100 */
    NULL,
    NULL,
    NULL,
    "bind",		/* #104 */
    "setsockopt",		/* #105 */
    "listen",		/* #106 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "gettimeofday",		/* #116 */
    "getrusage",		/* #117 */
    "getsockopt",		/* #118 */
    NULL,
    "readv",		/* #120 */
    "writev",		/* #121 */
    "settimeofday",		/* #122 */
    "fchown",		/* #123 */
    "fchmod",		/* #124 */
    NULL,
    "setreuid",		/* #126 */
    "setregid",		/* #127 */
    "rename",		/* #128 */
    NULL,
    NULL,
    "flock",		/* #131 */
    "mkfifo",		/* #132 */
    "sendto",		/* #133 */
    "shutdown",		/* #134 */
    "socketpair",		/* #135 */
    "mkdir",		/* #136 */
    "rmdir",		/* #137 */
    "utimes",		/* #138 */
    NULL,
    "adjtime",		/* #140 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "setsid",		/* #147 */
    "quotactl",		/* #148 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "nfssvc",		/* #155 */
    NULL,
    NULL,
    NULL,
    NULL,
    "lgetfh",		/* #160 */
    "getfh",		/* #161 */
    "getdomainname",		/* #162 */
    "setdomainname",		/* #163 */
    "uname",		/* #164 */
    "sysarch",		/* #165 */
    "rtprio",		/* #166 */
    NULL,
    NULL,
    "semsys",		/* #169 */
    "msgsys",		/* #170 */
    "shmsys",		/* #171 */
    NULL,
    "pread",		/* #173 */
    "pwrite",		/* #174 */
    NULL,
    "ntp_adjtime",		/* #176 */
    NULL,
    NULL,
    NULL,
    NULL,
    "setgid",		/* #181 */
    "setegid",		/* #182 */
    "seteuid",		/* #183 */
    NULL,
    NULL,
    NULL,
    NULL,
    "stat",		/* #188 */
    "fstat",		/* #189 */
    "lstat",		/* #190 */
    "pathconf",		/* #191 */
    "fpathconf",		/* #192 */
    NULL,
    "getrlimit",		/* #194 */
    "setrlimit",		/* #195 */
    "getdirentries",		/* #196 */
    "mmap",		/* #197 */
    "__syscall",		/* #198 */
    "lseek",		/* #199 */
    "truncate",		/* #200 */
    "ftruncate",		/* #201 */
    "__sysctl",		/* #202 */
    "mlock",		/* #203 */
    "munlock",		/* #204 */
    "undelete",		/* #205 */
    "futimes",		/* #206 */
    "getpgid",		/* #207 */
    NULL,
    "poll",		/* #209 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "__semctl",		/* #220 */
    "semget",		/* #221 */
    "semop",		/* #222 */
    NULL,
    "msgctl",		/* #224 */
    "msgget",		/* #225 */
    "msgsnd",		/* #226 */
    "msgrcv",		/* #227 */
    "shmat",		/* #228 */
    "shmctl",		/* #229 */
    "shmdt",		/* #230 */
    "shmget",		/* #231 */
    "clock_gettime",		/* #232 */
    "clock_settime",		/* #233 */
    "clock_getres",		/* #234 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "nanosleep",		/* #240 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "minherit",		/* #250 */
    "rfork",		/* #251 */
    "openbsd_poll",		/* #252 */
    "issetugid",		/* #253 */
    "lchown",		/* #254 */
    NULL,
    "NMLN",		/* #256 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "getdents",		/* #272 */
    NULL,
    "lchmod",		/* #274 */
    "netbsd_lchown",		/* #275 */
    "lutimes",		/* #276 */
    "netbsd_msync",		/* #277 */
    "nstat",		/* #278 */
    "nfstat",		/* #279 */
    "nlstat",		/* #280 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "fhopen",		/* #298 */
    "fhstat",		/* #299 */
    "modnext",		/* #300 */
    "modstat",		/* #301 */
    "modfnext",		/* #302 */
    "modfind",		/* #303 */
    "kldload",		/* #304 */
    "kldunload",		/* #305 */
    "kldfind",		/* #306 */
    "kldnext",		/* #307 */
    "kldstat",		/* #308 */
    "kldfirstmod",		/* #309 */
    "getsid",		/* #310 */
    "setresuid",		/* #311 */
    "setresgid",		/* #312 */
    NULL,
    "aio_return",		/* #314 */
    "aio_suspend",		/* #315 */
    "aio_cancel",		/* #316 */
    "aio_error",		/* #317 */
    "aio_read",		/* #318 */
    "aio_write",		/* #319 */
    "lio_listio",		/* #320 */
    "yield",		/* #321 */
    NULL,
    NULL,
    "mlockall",		/* #324 */
    "munlockall",		/* #325 */
    "__getcwd",		/* #326 */
    "sched_setparam",		/* #327 */
    "sched_getparam",		/* #328 */
    "sched_setscheduler",		/* #329 */
    "sched_getscheduler",		/* #330 */
    "sched_yield",		/* #331 */
    "sched_get_priority_max",		/* #332 */
    "sched_get_priority_min",		/* #333 */
    "sched_rr_get_interval",		/* #334 */
    "utrace",		/* #335 */
    NULL,
    "kldsym",		/* #337 */
    "jail",		/* #338 */
    NULL,
    "sigprocmask",		/* #340 */
    "sigsuspend",		/* #341 */
    NULL,
    "sigpending",		/* #343 */
    NULL,
    "sigtimedwait",		/* #345 */
    "sigwaitinfo",		/* #346 */
    "__acl_get_file",		/* #347 */
    "__acl_set_file",		/* #348 */
    "__acl_get_fd",		/* #349 */
    "__acl_set_fd",		/* #350 */
    "__acl_delete_file",		/* #351 */
    "__acl_delete_fd",		/* #352 */
    "__acl_aclcheck_file",		/* #353 */
    "__acl_aclcheck_fd",		/* #354 */
    "extattrctl",		/* #355 */
    "extattr_set_file",		/* #356 */
    "extattr_get_file",		/* #357 */
    "extattr_delete_file",		/* #358 */
    "aio_waitcomplete",		/* #359 */
    "getresuid",		/* #360 */
    "getresgid",		/* #361 */
    "kqueue",		/* #362 */
    "kevent",		/* #363 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "extattr_set_fd",		/* #371 */
    "extattr_get_fd",		/* #372 */
    "extattr_delete_fd",		/* #373 */
    "__setugid",		/* #374 */
    "nfsclnt",		/* #375 */
    "eaccess",		/* #376 */
    NULL,
    "nmount",		/* #378 */
    "kse_exit",		/* #379 */
    "kse_wakeup",		/* #380 */
    "kse_create",		/* #381 */
    "kse_thr_interrupt",		/* #382 */
    "kse_release",		/* #383 */
    "__mac_get_proc",		/* #384 */
    "__mac_set_proc",		/* #385 */
    "__mac_get_fd",		/* #386 */
    "__mac_get_file",		/* #387 */
    "__mac_set_fd",		/* #388 */
    "__mac_set_file",		/* #389 */
    "kenv",		/* #390 */
    "lchflags",		/* #391 */
    "uuidgen",		/* #392 */
    "sendfile",		/* #393 */
    "mac_syscall",		/* #394 */
    "getfsstat",		/* #395 */
    "statfs",		/* #396 */
    "fstatfs",		/* #397 */
    "fhstatfs",		/* #398 */
    NULL,
    "ksem_close",		/* #400 */
    "ksem_post",		/* #401 */
    "ksem_wait",		/* #402 */
    "ksem_trywait",		/* #403 */
    "ksem_init",		/* #404 */
    "ksem_open",		/* #405 */
    "ksem_unlink",		/* #406 */
    "ksem_getvalue",		/* #407 */
    "ksem_destroy",		/* #408 */
    "__mac_get_pid",		/* #409 */
    "__mac_get_link",		/* #410 */
    "__mac_set_link",		/* #411 */
    "extattr_set_link",		/* #412 */
    "extattr_get_link",		/* #413 */
    "extattr_delete_link",		/* #414 */
    "__mac_execve",		/* #415 */
    "sigaction",		/* #416 */
    "sigreturn",		/* #417 */
    NULL,
    NULL,
    NULL,
    "getcontext",		/* #421 */
    "setcontext",		/* #422 */
    "swapcontext",		/* #423 */
    "swapoff",		/* #424 */
    "__acl_get_link",		/* #425 */
    "__acl_set_link",		/* #426 */
    "__acl_delete_link",		/* #427 */
    "__acl_aclcheck_link",		/* #428 */
    "sigwait",		/* #429 */
    "thr_create",		/* #430 */
    "thr_exit",		/* #431 */
    "thr_self",		/* #432 */
    "thr_kill",		/* #433 */
    "_umtx_lock",		/* #434 */
    "_umtx_unlock",		/* #435 */
    "jail_attach",		/* #436 */
    "extattr_list_fd",		/* #437 */
    "extattr_list_file",		/* #438 */
    "extattr_list_link",		/* #439 */
    "kse_switchin",		/* #440 */
    "ksem_timedwait",		/* #441 */
    "thr_suspend",		/* #442 */
    "thr_wake",		/* #443 */
    "kldunloadf",		/* #444 */
    "MAXSYSCALL",		/* #445 */
  };

#define K_SYSMAX_SYSCALLS	445

#endif /* !__KSYSCALL_H__ */
