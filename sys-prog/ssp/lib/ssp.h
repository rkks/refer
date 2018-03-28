#ifndef _SSP_H
#define _SSP_H

#include <sys/types.h>		/* Required for some of our function prototypes */
#include <sys/termios.h>	/* Required for some of our function prototypes */

#define BUF_SIZE 8192
#define LINE_LEN 256
#define FILE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define DIR_PERMS (FILE_PERMS | S_IXUSR | S_IXGRP | S_IXOTH)
#define FIFO_PERMS (FILE_PERMS | S_IWGRP | S_IWOTH)

#define ssp_rlock(fd, whence, start, len) \
	ssp_lock (fd, F_SETLK, F_RDLCK, whence, start, len)
#define ssp_rlockw(fd, whence, start, len) \
	ssp_lock (fd, F_SETLKW, F_RDLCK, whence, start, len)
#define ssp_wlock(fd, whence, start, len) \
	ssp_lock (fd, F_SETLK, F_WRLCK, whence, start, len)
#define ssp_wlockw(fd, whence, start, len) \
	ssp_lock (fd, F_SETLKW, F_WRLCK, whence, start, len)
#define ssp_unlock(fd, whence, start, len) \
	ssp_lock (fd, F_SETLK, F_UNLCK, whence, start, len)

extern void err_dump (const char *text, ...);
extern void err_msg (const char *text, ...);
extern void err_quit (const char *text, ...);
extern void err_ret (const char *text, ...);
extern void log_msg (const char *text, ...);
extern int set_fsflag (int fd, int new_flags);
extern int clear_fsflag (int fd, int new_flags);
extern int ssp_lock (int fd, int cmd, short type, short whence, off_t start,
	off_t len);
extern ssize_t readn (int fd, void *buf, size_t num);
extern ssize_t writen (int fd, const void *buf, size_t num);
extern void print_term_status (int status);
#ifdef NEED_SNPRINTF
extern int snprintf (char *buf, size_t n, const char *fmt, ...);
#endif

extern int ssp_tell_wait_init (void);
extern int ssp_tell_child (pid_t pid);
extern int ssp_tell_parent (pid_t pid);
extern int ssp_wait_child (void);
extern int ssp_wait_parent (void);
extern int ssp_tell_wait_fin (void);
extern void daemon_init (const char *prog_name, int inetd);
extern int one_copy (const char *pid_file);
extern int send_fd (int pipe_fd, int fd);
extern int send_err (int pipe_fd, char status);
extern int recv_fd (int pipe_fd);
extern int srv_listen (const char *path);
extern int srv_accept (int fd, uid_t *uid);
extern int cli_connect (const char *path);
extern pid_t pty_fork (int *master_fdp, const struct termios *slave_termios,
	const struct winsize *slave_winsize);

#endif	/* _SSP_H */
