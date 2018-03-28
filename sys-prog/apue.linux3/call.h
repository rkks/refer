#include	<sys/types.h>
#include	<sys/time.h>
#include	<errno.h>
#include	<termios.h>
#include	"ourhdr.h"

#define	CS_CALL	"/home/stevens/calld"	/* well-known server name */
#define	CL_CALL	"call"					/* command for server */

				/* declare global variables */
extern char	 escapec;	/* tilde for local commands */
extern char	*src;		/* for take and put commands */
extern char	*dst;		/* for take and put commands */

				/* function prototypes */
int		call(const char *);
int		doescape(int);
void	loop(int);
int		prompt_read(char *, int (*)(int, char **));
void	put(int);
void	take(int);
int		take_put_args(int, char **);
