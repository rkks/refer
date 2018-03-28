#include	<sys/types.h>
#include	<errno.h>
#include	"ourhdr.h"

#define	CS_OPEN "/home/stevens/opend" /* well-known name */
#define	CL_OPEN "open"				  /* client's request for server */

			/* declare global variables */
extern int	 debug;		/* nonzero if interactive (not daemon) */
extern char	 errmsg[];	/* error message string to return to client */
extern int	 oflag;		/* open flag: O_xxx ... */
extern char	*pathname;	/* of file to open for client */

typedef struct {	/* one Client struct per connected client */
  int	fd;		/* fd, or -1 if available */
  uid_t	uid;
} Client;

extern Client	*client;		/* ptr to malloc'ed array */
extern int		 client_size;	/* # entries in client[] array */
					/* (both manipulated by client_XXX() functions) */

			/* function prototypes */
int		 cli_args(int, char **);
int		 client_add(int, uid_t);
void	 client_del(int);
void	 loop(void);
void	 request(char *, int, int, uid_t);
