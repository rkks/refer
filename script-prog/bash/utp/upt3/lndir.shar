From: koreth@ssyx.ucsc.edu (Steven Grimm)
Newsgroups: comp.sources.misc
Subject: v02i039: Symbolic directory copier ("lndir")
Message-ID: <7174@ncoast.UUCP>
Date: 3 Feb 88 02:01:29 GMT
Approved: allbery@ncoast.UUCP

Comp.sources.misc: Volume 2, Issue 39
Submitted-By: "Steven Grimm" <koreth@ssyx.ucsc.edu>
Archive-Name: lndir

This is my first posting to comp.sources.unix; I assume that a posting here
will go to the moderator.  If anyone gets this and I'm doing something wrong,
let me know.  Thanks.

[Yes, r$ is back, and bouncing stuff as readily as ever!  ;-)  ++bsa]


Steven Grimm

---------- cut here ----------
# This is a shell archive.  Remove anything before this line
# then unpack it by saving it in a file and typing "sh file"
# (Files unpacked will be owned by you and have default permissions).
# This archive contains the following files:
#	./lndir.c
#	./Makefile
#
if `test ! -s ./lndir.c`
then
echo "writing ./lndir.c"
cat > ./lndir.c << '\Rogue\Monster\'
#include <stdio.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>
char *rindex();

/*
** LNDIR
**
** Written by Steven Grimm (koreth@ssyx.ucsc.edu), 11-9-87
**
** A safe way to duplicate a directory structure elsewhere on the filesystem.
** It's necessary because a cd into a straight symbolic link actually changes
** to the directory pointed to by the link, which can be dangerous if the
** link is in a sensitive area of the filesystem.  LNDIR recursively recreates
** a directory structure, making symbolic links to all the files in the
** directory.
**
** This is really pretty ugly, but was the only way to solve a few problems
** we were having.
**
** Usage: lndir srcdir [destdir]
**   Where destdir is the name that you want the source directory to have
**   in its new location -- that is, to link /usr/src/foo to /usr/a/xxx/foo,
**   you'd do
**   % lndir /usr/src/foo /usr/a/xxx/foo
**   If no destination directory is specified, the new directory is given
**   the same name as its original and placed in the current directory, so
**   the above is equivalent to
**   % cd /usr/a/xxx
**   % lndir /usr/src/foo
*/

int errno;		/* System error number storage. */
char errfile[1024];	/* Filename that caused the error. */

main(argc, argv)
char **argv;
{
	char srcdir[1024], destdir[1024];

	if (argc < 2 || argc > 3)
	{
		printf("Usage: %s srcdir [destdir]\n", argv[0]);
		exit(-1);
	}

	strcpy(srcdir, argv[1]);
	if (argc == 2)
	{
		char *sdirname;

		strcpy(destdir, "./");
		sdirname = rindex(srcdir, '/');
		if (! sdirname)
		{
			printf("Can't copy a directory to itself.\n");
			exit(-1);
		}
		strcat(destdir, sdirname+1);
	}
	else
		strcpy(destdir, argv[2]);
	
	if ((errno = copydir(srcdir, destdir)) < 0)
	{
		char error[2048];
		sprintf(error, "File %s", errfile);
		perror(error);
	}
	exit(errno);
}

/*
** Create a copy of the source directory in the destination directory, and
** create symbolic links to all the files there.  If any of the files are
** directories themselves, recursively copy them into the destination
** directory using the same procedure (which is what recursive means,
** isn't it?)
*/
copydir(s, d)
char *s, *d;
{
	DIR		*dptr;
	struct direct	*file;
	struct stat	sbuf, sourcedir;

	printf("Copying %s to %s\n", s, d);

	if (stat(s, &sourcedir) == -1)
	{
		strcpy(errfile, s);
		return errno;
	}

	if ((dptr = opendir(s)) == NULL)
	{
		strcpy(errfile, s);
		return errno;
	}

/* The new directory is created with mode 700, so we can write to it (just in
   case the source directory isn't writeable, or umask is set to some weird
   value.)  It's chmodded to its original value after its contents have been
   copied. */
	if (mkdir(d, 0700) == -1)
	{
		strcpy(errfile, d);
		return errno;
	}

	while (file = readdir(dptr))
	{
		char	srcfile[1024], destfile[1024];

		strcpy(srcfile, s);
		strcat(srcfile, "/");
		strcat(srcfile, file->d_name);
		strcpy(destfile, d);
		strcat(destfile, "/");
		strcat(destfile, file->d_name);
		if (stat(srcfile, &sbuf) == -1)
		{
			strcpy(errfile, srcfile);
			return errno;
		}
		if (sbuf.st_mode & S_IFDIR)	/* Is this a directory? */
		{
			if (file->d_name[0] == '.' &&	/* Is it . or ..? */
			    (file->d_name[1] == '\0' ||
			     (file->d_name[1] == '.' &&
			      file->d_name[2] == '\0')))
				continue;
			if (errno = copydir(srcfile, destfile))
				return errno;
		}
		else
			if (symlink(srcfile, destfile) == -1)
			{
				strcpy(errfile, srcfile);
				return errno;
			}
	}
	if (chmod(d, sourcedir.st_mode & 07777) == -1)
	{
		strcpy(errfile, d);
		return errno;
	}
	return 0;
}

\Rogue\Monster\
else
  echo "will not over write ./lndir.c"
fi
if `test ! -s ./Makefile`
then
echo "writing ./Makefile"
cat > ./Makefile << '\Rogue\Monster\'
CFLAGS = -O

lndir: lndir.c
	$(CC) $(CFLAGS) lndir.c -o lndir
\Rogue\Monster\
else
  echo "will not over write ./Makefile"
fi
echo "Finished archive 1 of 1"
# if you want to concatenate archives, remove anything after this line
exit
+New! Improved! Now 100% Artificial-+-+-----------------------------------+
|#   #  @@@  ****  &&&&& $$$$$ %   %| |Steven Grimm                       |
|#  #  @   @ *   * &       $   %   %+-+ ARPA: koreth@ucscb.ucsc.edu       |
|###   @   @ ****  &&&&    $   %%%%%| | UUCP: ...!ucbvax!ucscc!ssyx!koreth|
|#  #  @   @ * *   &       $   %   %+-+     ______________________________|
|#   #  @@@  *  ** &&&&&   $   %   %| |     |"Let's see what's out there."|
+-----with NutraSour(TM)!  No natural colors or preservatives!------------+
