Newsgroups: comp.sources.misc
X-UNIX-From: sherpa!rac
organization: Personal System Computing,  St. Petersburg, FL
keywords: look
subject: v15i096: look - find strings in a sorted file
from: rac@sherpa.UUCP (Roger Cornelius)
Sender: allbery@uunet.UU.NET (Brandon S. Allbery - comp.sources.misc)

Posting-number: Volume 15, Issue 96
Submitted-by: rac@sherpa.UUCP (Roger Cornelius)
Archive-name: look_rac/part01

Here's a short, fairly fast, fairly portable version of look.  The
undocumented (at least here) -ttabchar option is not implemented, only
because I'm not positive what it does.  If someone with access to source
can describe its function, I'll add it.  I'm also interested to know
how this compares in performance to the original.

look searches a sorted file for lines beginning with a specified string.
See the README for details.

No manpage or makefile, just cc -O look.c -o look.

--
Roger A. Cornelius          rac@sherpa.UUCP         uunet!sherpa!rac


---- cut here ---
#! /bin/sh
# This is a shell archive.  Remove anything before this line, then unpack
# it by saving it into a file and typing "sh file".  To overwrite existing
# files, type "sh file -c".  You can also feed this as standard input via
# unshar, or by typing "sh <file", e.g..  If this archive is complete, you
# will see the following message at the end:
#		"End of shell archive."
# Contents:  README look.c
# Wrapped by rac@sherpa on Fri Oct 26 20:06:14 1990
PATH=/bin:/usr/bin:/usr/ucb ; export PATH
if test -f 'README' -a "${1}" != "-c" ; then 
  echo shar: Will not clobber existing file \"'README'\"
else
echo shar: Extracting \"'README'\" \(1032 characters\)
sed "s/^X//" >'README' <<'END_OF_FILE'
X
X    Here's a short, fairly fast, fairly portable version of look.  The
X    undocumented (at least here) -ttabchar option is not implemented,
X    only because I'm not positive what it does.  If someone with access
X    to source can describe its function, I'll add it.  I'm also interested
X    to know how this compares speedwise to the original.
X
X    Usage: look [-df] string [file]
X    file is assumed to be sorted.
X    The d and f options behave as in sort(1).
X    If -d is given, only letters, digits, tabs and blanks are compared.
X    If -f is given, uppercase letters are folded to lowercase - i.e.,
X    the search is case insensitive.
X    If no file is specified, -df and /usr/dict/words are assumed.
X    Return status: 0 if search was successful, 1 if not, and 2 on error.
X
X    To build, just cc -O look.c -o look. 
X    tolower() is assumed to do the right thing with non-alpha characters.
X    Bug reports/comments welcome.
X
X    Roger Cornelius
X    --
X    Roger A. Cornelius          rac@sherpa.UUCP         uunet!sherpa!rac
END_OF_FILE
if test 1032 -ne `wc -c <'README'`; then
    echo shar: \"'README'\" unpacked with wrong size!
fi
# end of 'README'
fi
if test -f 'look.c' -a "${1}" != "-c" ; then 
  echo shar: Will not clobber existing file \"'look.c'\"
else
echo shar: Extracting \"'look.c'\" \(4425 characters\)
sed "s/^X//" >'look.c' <<'END_OF_FILE'
X/*
X * look - search a sorted file for lines beginning with string
X *  Roger Cornelius (rac@sherpa.uucp)
X *  Fri Oct 26 13:13:54 EDT 1990
X *  public domain
X */
X
X#define _USE_MACROS     /* for SCO UNIX, see ctype.h */
X#include <stdio.h>
X#include <sys/types.h>
X#include <sys/stat.h>
X#include <string.h>
X#include <ctype.h>
X#define ERR 2
X
Xextern void exit();
Xint dstrncmp();
Xoff_t backup();
XFILE *fopen();
Xint fold = 0;
Xint dict = 0;
Xint (*compare)();
X
Xmain(argc, argv)
X    register int argc;
X    register char **argv;
X{
X    extern int optind;
X    char *file = "/usr/dict/words";
X    char *string;
X    int ch;
X    
X    while ((ch = getopt(argc, argv, "df?")) != -1)
X        switch(ch) {
X            case 'd':   dict = 1; break;
X            case 'f':   fold = 1; break;
X            case '?':   goto usage;
X        }
X    argc -= optind;
X    argv += optind;
X
X    if (argc < 1 || argc > 2) {
Xusage:  (void)fputs("Usage: look [-df] string [file]\n", stderr);
X        exit(ERR);
X    }
X    string = *argv;
X    if (*++argv)
X        file = *argv;
X    else
X        fold = dict = 1;
X
X    return search(string, file);
X}
X
X/*
X * search --
X *  search a sorted file for lines beginning with string
X *  adapted from binsearch(), K&R 2nd ed.
X */
Xstatic
Xsearch(string, file)
X    char *string;
X    char *file;
X{
X    FILE *fp;
X    char buf[BUFSIZ], save[BUFSIZ];
X    off_t low, mid, high, savemid = -1;
X    off_t found = -1;
X    register char *t;
X    struct stat sbuf;
X    size_t slen = strlen(string);
X    int n, res = 1;                 /* not found */
X
X    if ((fp = freopen(file, "r", stdin)) == NULL
X    || stat(file, &sbuf) != 0) {
X        /* avoid the printf kitchen sink */
X        (void) fputs("look: can't access ", stderr);
X        (void) fputs(file, stderr);
X        (void) fputs("\n", stderr);
X        return ERR;
X    }
X    low = 0;
X    high = sbuf.st_size;
X    compare = dict ? dstrncmp : strncmp;
X    if (fold)
X        for (t=string; *t; *t = tolower(*t), t++)
X            ;
X
X    while (low < high) {
X        mid = (low+high) / 2;
X        if (backup(&low, &mid, &high) == -1 || mid == savemid)
X            break;
X        savemid = mid;
X        if (fseek(fp, mid, 0) != 0 || gets(buf) == NULL)
X            return ERR;
X
X        if (fold)
X            for (t=buf; *t; *t = tolower(*t), t++)
X                ;
X        n = (*compare)(string, buf, slen);
X        if (n < 0)
X            high = mid - 1;
X        else if (n > 0)
X            low = mid + 1;
X        else {
X            found = mid;            /* save offset of found string */
X            high = mid - 1;
X            if (strlen(buf) == slen)
X                break;
X        }
X    }
X    if (found >= 0) {
X        if (fseek(fp, found, 0) != 0)
X            return ERR;
X        while (gets(buf) != NULL) {
X            if (fold) {
X                (void)strcpy(save, buf);
X                for (t=buf; *t; *t = tolower(*t), t++)
X                    ;
X            }
X            if ((*compare)(string, buf, slen) != 0)
X                break;
X            (void)puts(fold ? save : buf);
X        }
X        res = 0;                    /* found */
X    }
X    (void)fclose(fp);
X    return res;
X}
X
X/*
X * backup --
X *  search backwards for newline - if none found
X *  then reverse direction and start over
X */
Xstatic off_t
Xbackup(low, offset, high)
X    register off_t *low, *offset, *high;
X{
X    register int c;
X    register int inc = -1;
X    off_t save = *offset;
X
X    if (*offset == 0)
X        return(*offset);
X
X    for (; *offset < *high && fseek(stdin, *offset, 0) == 0; *offset += inc) {
X        c = getchar();
X        if (c == '\n' || *offset == 0)
X            return (*offset == 0) ? *offset : (*offset)++;
X        else if (c == EOF)      /* should never happen */
X            break;
X        if (*offset <= *low) {  /* start over, reversing direction */
X            *offset = save;
X            inc = 1;
X        }
X    }
X    return -1L;
X}
X
X/*
X * dstrncmp --
X *  dictionary strncmp() - only alpha characters,
X *  digits, blanks and tabs participate in comparisons
X */
Xstatic
Xdstrncmp(s1, s2, n)
X    register char *s1, *s2;
X    register size_t n;
X{
X    for (; n--; s1++, s2++) {
X        while (*s1 && !(isalpha(*s1) || isdigit(*s1) || isspace(*s1)))
X            s1++;
X        if (*s1 == '\0')
X            break;
X        while (*s2 && !(isalpha(*s2) || isdigit(*s2) || isspace(*s2)))
X            s2++;
X        if (*s1 != *s2)
X            return *s1 - *s2;
X    }
X    return 0;
X}
X
X/* end of look.c */
END_OF_FILE
if test 4425 -ne `wc -c <'look.c'`; then
    echo shar: \"'look.c'\" unpacked with wrong size!
fi
# end of 'look.c'
fi
echo shar: End of shell archive.
exit 0
-- 
Roger A. Cornelius          rac@sherpa.UUCP         uunet!sherpa!rac

