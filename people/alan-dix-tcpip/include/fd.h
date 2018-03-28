/****************************************/
/*                                      */
/*      fd.h                            */
/*                                      */
/*      Alan Dix                        */
/*                                      */
/*      5th January 1990                */
/*                                      */
/****************************************/

#ifndef FD_HEAD
#define FD_HEAD

#include "cpp.h"

#ifdef FD_SETSIZE

#define MAX_FD  FD_SETSIZE

#ifndef FD_ISEMPTY   /*  fix for SUN OS which doesn't have this macro */
                     /*  N.B. see warning below for !OLD_SELECT       */
#define FD_ISEMPTY(p)   ((p)->fds_bits[0] == 0)
#endif

#else /* FD_SETSIZE undefined */

/*  needs <sys/types.h> and <sys/time.h>  */

#ifdef OLD_SELECT
        /*  for compatibility with old UNIXs which didn't define     */
        /*  these macros.                                            */

#define FD_ZERO(p)      (*(p) = 0)
#define FD_SET(n,p)     (*(p) |= (1<<(n)))
#define FD_CLEAR(n,p)   (*(p) &=~(1<<(n)))
#define FD_ISSET(n,p)   (*(p) & (1<<(n)))
#define FD_ISEMPTY(p)   (*(p) == 0)

#else
	/*  N.B. theses macros only work when there are a small      */
        /*  number of file descriptors (typically 32) see your own   */
        /*  UNIX sys/types.h file for the correct versions and use   */
        /*  it instead                                               */
#define FD_ZERO(p)      ((p)->fds_bits[0] = 0)
#define FD_SET(n,p)     ((p)->fds_bits[0] |= (1<<(n)))
#define FD_CLEAR(n,p)   ((p)->fds_bits[0] &=~(1<<(n)))
#define FD_ISSET(n,p)   ((p)->fds_bits[0] & (1<<(n)))
#define FD_ISEMPTY(p)   ((p)->fds_bits[0] == 0)

#endif  /* OLD_SELECT */

#define MAX_FD  32   /* should be  getdtablesize()  */

#endif /* FD_SETSIZE */

#define OK_FD(fd)  ( 0<= (fd) && (fd) < MAX_FD )

#ifdef ANSI
OLDC_FUN int select( int width, fd_set *readfds, fd_set *writefds,
            fd_set *exceptfds, const struct timeval *timout );
#else
int select();
#endif

#endif  /* FD_HEAD */
