/* spname:  return correctly spelled filename */
/*
 * spname(oldname, newname)  char *oldname, *newname;
 *	returns -1 if no reasonable match to oldname,
 *	         0 if exact match,
 *	         1 if corrected.
 *	stores corrected name in newname.
 */

#include <sys/types.h>
#include <sys/dir.h>

spname(oldname, newname)
	char *oldname, *newname;
{
	char *p, guess[DIRSIZ+1], best[DIRSIZ+1];
	char *new = newname, *old = oldname;

	for (;;) {
		while (*old == '/')	/* skip slashes */
			*new++ = *old++;
		*new = '\0';
		if (*old == '\0')	/* exact or corrected */
			return strcmp(oldname,newname) != 0;
		p = guess;	/* copy next component into guess */
		for ( ; *old != '/' && *old != '\0'; old++)
			if (p < guess+DIRSIZ)
				*p++ = *old;
		*p = '\0';
		if (mindist(newname, guess, best) >= 3)
			return -1;	/* hopeless */
		for (p = best; *new = *p++; ) /* add to end */
			new++;					  /* of newname */
	}
}

mindist(dir, guess, best)	/* search dir for guess */
	char *dir, *guess, *best;
{
	/* set best, return distance 0..3 */
	int d, nd, fd;
	struct {
		ino_t ino;
		char  name[DIRSIZ+1];	/* 1 more than in dir.h */
	} nbuf;

	nbuf.name[DIRSIZ] = '\0';	/* +1 for terminal '\0' */
	if (dir[0] == '\0')		/* current directory */
		dir = ".";
	d = 3;	/* minimum distance */
	if ((fd=open(dir, 0)) == -1)
		return d;
	while (read(fd,(char *) &nbuf,sizeof(struct direct)) > 0)
		if (nbuf.ino) {
			nd = spdist(nbuf.name, guess);
			if (nd <= d && nd != 3) {
				strcpy(best, nbuf.name);
				d = nd;
				if (d == 0)		/* exact match */
					break;
			}
		}
	close(fd);
	return d;
}

/* spdist:  return distance between two names */
/*
 *	very rough spelling metric:
 *	0 if the strings are identical
 *	1 if two chars are transposed
 *	2 if one char wrong, added or deleted
 *	3 otherwise
 */

#define	EQ(s,t)	(strcmp(s,t) == 0)

spdist(s, t)
	char *s, *t;
{
	while (*s++ == *t)
		if (*t++ == '\0')
			return 0;		/* exact match */
	if (*--s) {
		if (*t) {
			if (s[1] && t[1] && *s == t[1] 
			  && *t == s[1] && EQ(s+2, t+2))
				return 1;	/* transposition */
			if (EQ(s+1, t+1))
				return 2;	/* 1 char mismatch */
		}
		if (EQ(s+1, t))
			return 2;		/* extra character */
	}
	if (*t && EQ(s, t+1))
		return 2;			/* missing character */
	return 3;
}
