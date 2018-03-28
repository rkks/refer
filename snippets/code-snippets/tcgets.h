/* Freeware */
#ifndef __TCGETS_H__
#define __TCGETS_H__

#ifndef DEFAULT_TTY_TIMEOUT
#define DEFAULT_TTY_TIMEOUT	5
#endif

#ifndef DEFAULT_TTY_SETTINGS
#define DEFAULT_TTY_SETTINGS "19200N81"
#endif

#ifndef DEFAULT_TERM
#define DEFAULT_TERM	"/dev/ttyS0"
#endif

#ifdef __cplusplus
extern "C" {
#endif


int termios_save(int fd);
int termios_restore(int fd);
int termios_defaults(int fd);
int termios_set(int fd, const char *cmdstr);
int termios_open(const char *fname, const char *cmdstr);

#ifdef __cplusplus
}
#endif

#endif/*__TCGETS_H__*/
