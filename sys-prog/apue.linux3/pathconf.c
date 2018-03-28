#include	<sys/types.h>
#include	<sys/param.h>
#include	<sys/stat.h>
#include	<errno.h>
#include	<limits.h>
#include	<unistd.h>

static struct stat	statbuf;

/* We don't check that the file for _PC_MAX_CANON really refers to a
 * terminal device, and so on.  POSIX.1 says that "it is unspecified
 * whether an implementation supports an association of the variable
 * name with the specified file."  Also, things like PATH_MAX aren't
 * correct if the specified file is really an SVR4 file on a "s5"
 * filesystem that has been NFS mounted ...  To check for these
 * details really requires that this code be in the kernel.
 */

static long
_do_pathconf(int name)
{
	switch (name) {
	case _PC_LINK_MAX:	return(LINK_MAX);
	case _PC_MAX_CANON:	return(MAX_CANON);
	case _PC_MAX_INPUT:	return(MAX_INPUT);
	case _PC_NAME_MAX:	return(NAME_MAX);
	case _PC_PATH_MAX:	return(PATH_MAX);
	case _PC_PIPE_BUF:	return(PIPE_BUF);
	case _PC_VDISABLE:	return(_POSIX_VDISABLE);

	case _PC_CHOWN_RESTRICTED:
#ifdef	_POSIX_CHOWN_RESTRICTED	/* always true for BSD */
		return(1);
#else
		return(-1);
#endif

	case _PC_NO_TRUNC:
#ifdef	_POSIX_NO_TRUNC		/* always true for BSD */
		return(1);
#else
		return(-1);
#endif

	default:	
		errno = EINVAL;
		return(-1);
	}
}

long
pathconf(const char *path, int name)
{
	if (stat(path, &statbuf) < 0)
		return(-1);	/* stat sets errno */
	return(_do_pathconf(name));
}

long
fpathconf(int fd, int name)
{
	if (fstat(fd, &statbuf) < 0)
		return(-1);	/* stat sets errno */
	return(_do_pathconf(name));
}
