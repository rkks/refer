/* This is an auto-generated file, do not edit */
/* SYSCALLS NAMES */

#ifndef __KSYSCALL_H__
#define __KSYSCALL_H__

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
    "#17",
    "getfsstat",		/* #18 */
    "#19",
    "getpid",		/* #20 */
    "#21",
    "#22",
    "setuid",		/* #23 */
    "getuid",		/* #24 */
    "geteuid",		/* #25 */
    "ptrace",		/* #26 */
    "recvmsg",		/* #27 */
    "sendmsg",		/* #28 */
    "recvfrom",		/* #29 */
    "accept",		/* #30 */
    "getpeername",		/* #31 */
    "getsockname",		/* #32 */
    "access",		/* #33 */
    "chflags",		/* #34 */
    "fchflags",		/* #35 */
    "sync",		/* #36 */
    "kill",		/* #37 */
    "#38",
    "getppid",		/* #39 */
    "#40",
    "dup",		/* #41 */
    "pipe",		/* #42 */
    "getegid",		/* #43 */
    "profil",		/* #44 */
    "ktrace",		/* #45 */
    "sigaction",		/* #46 */
    "getgid",		/* #47 */
    "sigprocmask",		/* #48 */
    "getlogin",		/* #49 */
    "setlogin",		/* #50 */
    "acct",		/* #51 */
    "sigpending",		/* #52 */
    "sigaltstack",		/* #53 */
    "ioctl",		/* #54 */
    "reboot",		/* #55 */
    "revoke",		/* #56 */
    "symlink",		/* #57 */
    "readlink",		/* #58 */
    "execve",		/* #59 */
    "umask",		/* #60 */
    "chroot",		/* #61 */
    "#62",
    "#63",
    "#64",
    "msync",		/* #65 */
    "vfork",		/* #66 */
    "#67",
    "#68",
    "sbrk",		/* #69 */
    "sstk",		/* #70 */
    "#71",
    "#72",
    "munmap",		/* #73 */
    "mprotect",		/* #74 */
    "madvise",		/* #75 */
    "#76",
    "#77",
    "mincore",		/* #78 */
    "getgroups",		/* #79 */
    "setgroups",		/* #80 */
    "getpgrp",		/* #81 */
    "setpgid",		/* #82 */
    "setitimer",		/* #83 */
    "#84",
    "swapon",		/* #85 */
    "getitimer",		/* #86 */
    "#87",
    "#88",
    "getdtablesize",		/* #89 */
    "dup2",		/* #90 */
    "#91",
    "fcntl",		/* #92 */
    "select",		/* #93 */
    "#94",
    "fsync",		/* #95 */
    "setpriority",		/* #96 */
    "socket",		/* #97 */
    "connect",		/* #98 */
    "#99",
    "getpriority",		/* #100 */
    "#101",
    "#102",
    "sigreturn",		/* #103 */
    "bind",		/* #104 */
    "setsockopt",		/* #105 */
    "listen",		/* #106 */
    "#107",
    "#108",
    "#109",
    "#110",
    "sigsuspend",		/* #111 */
    "#112",
    "#113",
    "#114",
    "#115",
    "gettimeofday",		/* #116 */
    "getrusage",		/* #117 */
    "getsockopt",		/* #118 */
    "#119",
    "readv",		/* #120 */
    "writev",		/* #121 */
    "settimeofday",		/* #122 */
    "fchown",		/* #123 */
    "fchmod",		/* #124 */
    "#125",
    "#126",
    "#127",
    "rename",		/* #128 */
    "#129",
    "#130",
    "flock",		/* #131 */
    "mkfifo",		/* #132 */
    "sendto",		/* #133 */
    "shutdown",		/* #134 */
    "socketpair",		/* #135 */
    "mkdir",		/* #136 */
    "rmdir",		/* #137 */
    "utimes",		/* #138 */
    "futimes",		/* #139 */
    "adjtime",		/* #140 */
    "#141",
    "#142",
    "#143",
    "#144",
    "#145",
    "#146",
    "setsid",		/* #147 */
    "#148",
    "#149",
    "#150",
    "getpgid",		/* #151 */
    "setprivexec",		/* #152 */
    "pread",		/* #153 */
    "pwrite",		/* #154 */
    "nfssvc",		/* #155 */
    "#156",
    "statfs",		/* #157 */
    "fstatfs",		/* #158 */
    "unmount",		/* #159 */
    "#160",
    "getfh",		/* #161 */
    "#162",
    "#163",
    "#164",
    "quotactl",		/* #165 */
    "#166",
    "mount",		/* #167 */
    "#168",
    "#169",
    "table",		/* #170 */
    "#171",
    "#172",
    "#173",
    "#174",
    "gc_control",		/* #175 */
    "add_profil",		/* #176 */
    "#177",
    "#178",
    "#179",
    "kdebug_trace",		/* #180 */
    "setgid",		/* #181 */
    "setegid",		/* #182 */
    "seteuid",		/* #183 */
    "sigreturn",		/* #184 */
    "#185",
    "#186",
    "#187",
    "stat",		/* #188 */
    "fstat",		/* #189 */
    "lstat",		/* #190 */
    "pathconf",		/* #191 */
    "fpathconf",		/* #192 */
    "getfsstat",		/* #193 */
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
    "ATsocket",		/* #206 */
    "ATgetmsg",		/* #207 */
    "ATputmsg",		/* #208 */
    "ATPsndreq",		/* #209 */
    "ATPsndrsp",		/* #210 */
    "ATPgetreq",		/* #211 */
    "ATPgetrsp",		/* #212 */
    "#213",
    "kqueue_from_portset_np",		/* #214 */
    "kqueue_portset_np",		/* #215 */
    "mkcomplex",		/* #216 */
    "statv",		/* #217 */
    "lstatv",		/* #218 */
    "fstatv",		/* #219 */
    "getattrlist",		/* #220 */
    "setattrlist",		/* #221 */
    "getdirentriesattr",		/* #222 */
    "exchangedata",		/* #223 */
    "checkuseraccess",		/* #224 */
    "searchfs",		/* #225 */
    "#226",
    "#227",
    "#228",
    "#229",
    "#230",
    "#231",
    "#232",
    "#233",
    "#234",
    "#235",
    "#236",
    "#237",
    "#238",
    "#239",
    "#240",
    "#241",
    "fsctl",		/* #242 */
    "#243",
    "#244",
    "#245",
    "#246",
    "nfsclnt",		/* #247 */
    "fhopen",		/* #248 */
    "#249",
    "minherit",		/* #250 */
    "semsys",		/* #251 */
    "msgsys",		/* #252 */
    "shmsys",		/* #253 */
    "semctl",		/* #254 */
    "semget",		/* #255 */
    "semop",		/* #256 */
    "semconfig",		/* #257 */
    "msgctl",		/* #258 */
    "msgget",		/* #259 */
    "msgsnd",		/* #260 */
    "msgrcv",		/* #261 */
    "shmat",		/* #262 */
    "shmctl",		/* #263 */
    "shmdt",		/* #264 */
    "shmget",		/* #265 */
    "shm_open",		/* #266 */
    "shm_unlink",		/* #267 */
    "sem_open",		/* #268 */
    "sem_close",		/* #269 */
    "sem_unlink",		/* #270 */
    "sem_wait",		/* #271 */
    "sem_trywait",		/* #272 */
    "sem_post",		/* #273 */
    "sem_getvalue",		/* #274 */
    "sem_init",		/* #275 */
    "sem_destroy",		/* #276 */
    "#277",
    "#278",
    "#279",
    "#280",
    "#281",
    "#282",
    "#283",
    "#284",
    "#285",
    "#286",
    "#287",
    "#288",
    "#289",
    "#290",
    "#291",
    "#292",
    "#293",
    "#294",
    "#295",
    "load_shared_file",		/* #296 */
    "reset_shared_file",		/* #297 */
    "new_system_shared_regions",		/* #298 */
    "#299",
    "#300",
    "#301",
    "#302",
    "#303",
    "#304",
    "#305",
    "#306",
    "#307",
    "#308",
    "#309",
    "getsid",		/* #310 */
    "#311",
    "#312",
    "aio_fsync",		/* #313 */
    "aio_return",		/* #314 */
    "aio_suspend",		/* #315 */
    "aio_cancel",		/* #316 */
    "aio_error",		/* #317 */
    "aio_read",		/* #318 */
    "aio_write",		/* #319 */
    "lio_listio",		/* #320 */
    "#321",
    "#322",
    "#323",
    "mlockall",		/* #324 */
    "munlockall",		/* #325 */
    "#326",
    "issetugid",		/* #327 */
    "__pthread_kill",		/* #328 */
    "pthread_sigmask",		/* #329 */
    "sigwait",		/* #330 */
    "#331",
    "#332",
    "#333",
    "#334",
    "#335",
    "#336",
    "#337",
    "#338",
    "#339",
    "#340",
    "#341",
    "#342",
    "#343",
    "#344",
    "#345",
    "#346",
    "#347",
    "#348",
    "#349",
    "audit",		/* #350 */
    "auditon",		/* #351 */
    "auditsvc",		/* #352 */
    "getauid",		/* #353 */
    "setauid",		/* #354 */
    "getaudit",		/* #355 */
    "setaudit",		/* #356 */
    "getaudit_addr",		/* #357 */
    "setaudit_addr",		/* #358 */
    "auditctl",		/* #359 */
    "#360",
    "#361",
    "kqueue",		/* #362 */
    "kevent",		/* #363 */
  };

#define K_SYSMAX_SYSCALLS	363

#endif /* !__KSYSCALL_H__ */
