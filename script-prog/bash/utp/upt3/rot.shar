Subject:  v15i030:  Rotate text
Newsgroups: comp.sources.unix
Sender: sources
Approved: rsalz@uunet.UU.NET

Submitted-by: Martin Schmidt <exunido.irb.informatik.uni-dortmund.de!mschmidt>
Posting-number: Volume 15, Issue 30
Archive-name: rot

This program rotates a file, so that lines become columns and vice versa.
Without any options, the file will be rotated clockwise. So, from the input

          abcde
          abcd
          abc
          ab
          a
             xx xx
          A
          AB
          ABC
          ABCD
          ABCDE

you will get

          AAAAA aaaaa
          BBBB   bbbb
          CCC     ccc
          DD   x   dd
          E    x    e

               x
               x

# This is a shell archive.  Unpack with "sh <file".
# Contents: Makefile rot.man rot.c
#
echo x - Makefile
cat > "Makefile" << '!Funky!Stuff!'
# Makefile for rot.c
#
# Edit this to your standards:
CFLAGS= -O
BINDIR=/usr/local
MANDIR=/usr/man/man1
# If you have preformatted manuals, remove the comment character
# in the install section
CATMANDIR=/usr/man/cat1
#
rot: rot.c
	cc $(CFLAGS) rot.c -o rot
rot.cat: rot.man
	tbl rot.man | nroff -man | col > rot.cat

install: rot rot.man rot.cat
	chmod 644 rot.man rot.cat
	mv rot.man $(MANDIR)/rot.1
#	mv rot.cat $(CATMANDIR)/rot.1
	chmod 711 rot
	mv rot $(BINDIR)
!Funky!Stuff!
echo x - rot.man
cat > "rot.man" << '!Funky!Stuff!'
.\"@(#) rot.man 88/03/03 -- use tbl
.TH ROT 1 public UNIX
.SH NAME
rot \- rotate a file
.SH SYNOPSIS
rot [-rbloB] [-c \fIc\fP] [\fIfile\fP]
.SH DESCRIPTION
.I Rot
rotates a file, so that lines become columns and vice versa. Without any
options, the file will be rotated clockwise. So, from the input
.nf
.in +10

abcde
abcd
abc
ab
a
\0\0\0xx\0xx
A
AB
ABC
ABCD
ABCDE

.ti -10
you will get

AAAAA\0aaaaa
BBBB\0\0\0bbbb
CCC\0\0\0\0\0ccc
DD\0\0\0x\0\0\0dd
E\0\0\0\0x\0\0\0\0e
\0\0\0\0\0\0
\0\0\0\0\0x
\0\0\0\0\0x

.in -10
.fi
.I Rot
uses a two pass algorithm, where the first part collects the line length of each
line in the input, and the second prints out the new lines, seeking through
the input file.
When no file or - is given,
.I rot
reads from standard input.
.SH OPTIONS
.TP
.B -r
rotate reverse (counter-clockwise)
.TP
.B -b
normally,
.I rot
suppresses printing of trailing blanks in output lines.
This option retains them, so all blanks of the input will appear
in the output
.TP
.B -l
do only the first pass and print the number of lines and the length of the
longest line found to standard output
.TP
.B -o
change order of columns; in combination with the rotation this yields
one more file operation (see below)
.TP
.B -B
this is for big files; the data will be hold in temporary files rather
than in memory; this slows down
.I rot
enormously
.TP
.B -c \fIc\fP
use
.I c
as "line"-separator, instead of the newline character
.br
.ne 8
.SH REMARKS
One may ask, what
.I rot
is useful for.
Think about the functionality of programs like
.IR cat (1),
or the line oriented
.IR grep (1),
.IR cut (1),
.IR sed (1),
.IR sort (1),
and others, when applied to a rotated file.
In conjunction with
.I rot
they do not longer work on lines, but columns.
For example
.P
.ti +10
rot f | grep ... | rot -r
.P
is a
.I grep
on columns of the file f.
.P
The -o option permits you to manipulate a file in some way of "reflecting" it
in a "diagonal" line. The following table shows the different effects of
.I rot
called with various options. Data manipulation is shown by means of the
output of a file with the contents
.P
.ti +10
AB
.br
.ti +10
CD
.P
and a symbolic notation. R means Rotation (angle given), M means
reflection at given mirror axis.
Combinations of two calls of
.I rot
are also given. (Other combinations
have identical effects to one of the mentioned.)

.TS
tab(#) center box;
lfB | cfB | cfB
l | c | c.
call#output#function
.sp 0.5
=
rot#CA#R
\^#DB#-90 deg
_
rot -r#BD#R
\^#AC#90 deg
_
rot -o#DB#M
\^#CA#/
_
rot -ro#AC#M
\^#BD#\e
=
rot | rot#DC#R
\^#BA#180 deg
_
rot | rot -r#AB#(Id) *
\^#CD#\^
_
rot | rot -o#BA#M **
\^#DC#|
_
rot | rot -ro#CD#M
\^#AB#\-
.TE

* Note, that  rot|rot -r  (or  rot|rot|rot|rot)  is not exactly the null
operator.
.I Rot
must insert blanks to keep track of the columns.
They may appear at the end of lines in a further run of
.IR rot ,
if the -b option is given.
Without this option blanks from the original input may be lost.
.P
** This is not the same as what is done by
.IR rev (1).
Rev does not retain columns!

.I Rot
knows nothing about tab characters, use
.IR expand (1)
to handle them correctly.
.SH FILES
.TP
/tmp/rot* -
temporarily copied input, if standard input is incapable of seeking
(pipe or terminal) (-B option only)
.TP
/tmp/rod* -
tempfile for holding length of input lines (-B option only)
.SH SEE ALSO
rev(1), tail(1bsd), tac(1public), expand(1), colrm(1)
.SH AUTHOR
.nf
Martin Schmidt
Dortmund - W. Germany
mschmidt@exunido.uucp
.fi
!Funky!Stuff!
echo x - rot.c
cat > "rot.c" << '!Funky!Stuff!'

                      /**************************/
                      /*****     rot.c      *****/
                      /*****    -------     *****/
                      /*****  Version 1.2   *****/
                      /***** -------------  *****/
                      /*****   16.02.1988   *****/
                      /**************************/


char what_id [] = "@(#) rot.c 1.2, M. Schmidt [1988/02/16]";

#include <stdio.h>
#include <signal.h>

#define MEMPIECE     8192L

char *tmpdata = "/tmp/rodXXXXXX";       /* numbers     */
char *tmpin = "/tmp/rotXXXXXX";         /* temp. stdin */
char *bigmsg = "rot: file too big (%s), use -B flag\n";
char nl;
FILE *ftmpdata, *ftmpin;
int lflag, rflag, bflag, oflag, Bflag, szl, cpin;
long holdbl, *lsptr, *lptr, *leptr;
char *fname, *isptr, *iptr, *ieptr;
extern int errno;
extern char *malloc(), *realloc();
char getinput();

main (argc, argv)
char **argv;
{  long maxline, col;
   int i;
   long nextlong(), pass1();
   int on_irupt();

   lflag = rflag = bflag = oflag = Bflag = cpin = 0;
   fname = "";
   nl = '\n';
   umask (077);
   szl = sizeof (long);

   i = 1;
   while (i < argc)
   {  if (*(argv[i]) == '-')
         do
         switch ( *(++(argv[i])) )
         {  case 'r' : rflag++; break;
            case 'l' : lflag++; break;
            case 'b' : bflag++; break;
            case 'o' : oflag++; break;
            case 'B' : Bflag++; break;
            case 'c' : nl = *(argv[++i]); break;
            case '\0': fname = ""; break;
            default  : fprintf (stderr, "usage: rot [-rbloB] [-c c] [file]\n");
                       exit (1);
         }
         while (*(argv[i]) != '\0');
      else
        if (*fname != '\0')
        {  fprintf (stderr, "rot: can rotate only one file\n");
           exit (1);
        }
        else
           fname = argv[i];
      i++;
   }

   if (signal (SIGINT, on_irupt) == SIG_IGN)
      signal (SIGINT, SIG_IGN);
   if (signal (SIGTERM, on_irupt) == SIG_IGN)
      signal (SIGTERM, SIG_IGN);
   if (signal (SIGHUP, on_irupt) == SIG_IGN)
      signal (SIGHUP, SIG_IGN);
   if (signal (SIGQUIT, on_irupt) == SIG_IGN)
      signal (SIGQUIT, SIG_IGN);
   signal (SIGSEGV, on_irupt);

   if ( Bflag && *fname == '\0' && fseek (stdin, 0L, 0) != 0)
      cpin++;                                     /* stdin is pipe */

   if (*fname != '\0')
      if (freopen (fname, "r", stdin) == NULL)
      {  fprintf (stderr, "rot: can't open file %s\n", fname);
         exit (1);
      }

   if ( Bflag && isatty (fileno(stdin)))
      cpin++;

   if (cpin) copyin();

   if (Bflag)
   {  mktemp (tmpdata);
      if ((ftmpdata = fopen (tmpdata, "w+")) == NULL)
      {  fprintf (stderr, "rot: can't create tempfile\n");
         exit (1);
      }
   }

   maxline = pass1 ();

   for (col = 1L; col <= maxline; col++)
   {
      if (rflag)
         rseekcolumn (oflag ? col : maxline + 1 - col);
      else
         seekcolumn (oflag ? maxline + 1 - col : col);

      putchar (nl);                    /* one line finished */
   }

   if (Bflag)
   {  fclose (ftmpdata);
      unlink (tmpdata);
   }
   if (cpin)
   {  fclose (ftmpin);
      unlink (tmpin);
   }
   exit (0);
}  /* main */


long
pass1 ()                       /* save length of lines in memory/tempfile     */
                               /*
                                * C-experts should have a close look at this.
                                * Pointer arithmetic is done without explicit
                                * use of arrays. Maybe non-portable. With
                                * big files memory segmentation may be
                                * significant (?). Segmentation-fault-signal is
                                * caught.
                                */
{  long max, z, lines, ccount, *lp;
   int c, tabseen = 0, lallocs, iallocs;
   char *ip;

   z = max = lines = ccount = 0L;

   if (!Bflag && !lflag)                  /* allocs for linelengths and input */
   {  lsptr = lp = (long *) malloc ((unsigned) MEMPIECE);
      if (lsptr == NULL)
      {  fprintf (stderr, bigmsg, "not enough memory");
         exit (1);
      }
      lallocs = 1;
      leptr = lsptr + ((long) lallocs) * (MEMPIECE/szl) - 1L;

      isptr = ip = malloc ((unsigned) (2 * MEMPIECE));
      if (isptr == NULL)
      {  fprintf (stderr, bigmsg, "not enough memory");
         exit (1);
      }
      iallocs = 2;
      ieptr = isptr + ((long) iallocs) * MEMPIECE - 1L;
   }

   while ((c = getchar()) != EOF)
   {  if (c == nl)
      {  if (!lflag)
         {  if (Bflag)                       /* write linelength in tempfile */
               fwrite ((char *) &z, szl, 1, ftmpdata);
            else                             /* put linelength into memory */
            {  if ( ((long) lp) > ((long) leptr) )
               {  lallocs++;                 /* more alloc */
                  if ((lsptr = (long *)
                     realloc (lsptr, (unsigned) (lallocs * MEMPIECE) )) == NULL)
                  {  fprintf (stderr, bigmsg, "not enough memory");
                     exit (1);
                  }
                  lp = lsptr + lines;
                  leptr = lsptr + ((long) lallocs) * (MEMPIECE/szl) - 1L;
               }
               *(lp++) = z;
            }
         }
         if (z > max) max = z;              /* find max. linelength */
         z = 0L;
         lines++;
      }  /* c == nl */
      else
      {  if (c == '\t') tabseen++;
         z++;
      }
      if (!Bflag && !lflag)                   /* put stdin into memory */
      {  if (((long) ip) > ((long) ieptr))
         {  iallocs += 2;                     /* more alloc */
            if ((isptr =
                 realloc (isptr, (unsigned) (iallocs * MEMPIECE) )) == NULL)
            {  fprintf (stderr, bigmsg, "not enough memory");
               exit (1);
            }
            ip = isptr + ccount;
            ieptr = isptr + ((long) iallocs) * MEMPIECE - 1L;
         }
         *(ip++) = c;                         /* put inputchar into memory */
         ccount++;
      }
   }  /* while */

   if (!Bflag && !lflag)
   { leptr = lp - 1;                          /* mark end of memory sections */
     ieptr = ip - 1;
   }

   if (tabseen) fprintf (stderr, "rot: don't like tab characters!\n");

   if (lflag)
   {  printf ("%ld %ld\n", lines, max);
      if (cpin) unlink (tmpin);
      exit (0);
   }

   return max;
}  /* pass1 */


rseekcolumn (col)
long col;
{  register long i2, sum, z;
   char c;
   long nextlong();

   seekinput (0L, 0);                /* goto begin of input */
   if (Bflag)
      fseek (ftmpdata, 0L, 0);       /* goto begin of temp */
   else
      lptr = lsptr;                  /* goto begin of list */

   holdbl = 0L;
   sum = - 1L;                       /* first seekinput() modified */
   i2 = nextlong ();

   while (i2 >= 0L)                 /* i2 == -1L means EOF on temp or stdin */
   {  z = 0L;
      while (i2 < col && i2 >= 0L)  /* skip lines shorter col */
      {  sum += i2;                 /* sum up linelengths */
         z++;                       /* z: number of nl's skipped */
         holdbl++;                  /* skipped lines -> blanks */
         i2 = nextlong ();          /* read next line length */
      }

      if (i2 >= 0L)
      {
         if (seekinput (sum + z + col, 1) == -1)
            return;

         if ((c = getinput()) == EOF)
            return;                  /* end of input */

         if (c == ' ' && !bflag)
            holdbl++;
         else
         {  putblanks (holdbl);
            holdbl = 0L;
            putchar (c);
         }

         sum = i2 - col;           /* rest of line or compensate col in fseek */
         i2 = nextlong ();         /* read length of next line */
      }
   }
}  /* rseekcolumn */


seekcolumn (col)
long col;
{  register long i2, sum, z;
   char c;
   long nextlong();

   seekinput (-1L, 2);               /* goto end of input */
   if (Bflag)
      fseek (ftmpdata, (long) szl, 2);
   else                              /* goto end of temp */
      lptr = leptr;                  /* goto end of list */

   holdbl = 0L;
   i2 = nextlong ();
   sum = i2 - col - 1L;             /* first seekinput() modified */

   while (i2 >= 0L)                 /* i2 == -1L means EOF on temp */
   {  z = 0L;
      while (i2 < col && i2 >= 0L)  /* skip lines shorter col */
      {  i2 = nextlong ();          /* read next line length */
         sum += i2;                 /* sum up linelengths */
         z++;                       /* z: number of nl's skipped */
         holdbl++;                  /* skipped line  -> blank */
      }

      if (i2 > 0L)
      {
         if (seekinput (- (sum + z + 2L), 1) == -1)
            return;
         if ((c = getinput()) == EOF)
            return;                  /* end of input */

         if (c == ' ' && !bflag)
            holdbl++;
         else
         {  putblanks (holdbl);
            holdbl = 0L;
            putchar (c);
         }
         sum = i2 = nextlong ();        /* read length of previous line */
      }
   }
}  /* seekcolumn */


putblanks (n)
long n;
{
   for (; n > 0L; n--) putchar (' ');
}  /* putblanks */


long
nextlong ()                                /* -1L = "EOF" */
{  long i;

   if (!Bflag)                             /* get from memory */
   {  if (lptr > leptr || lptr < lsptr)
         i = -1L;
      else
         i = *lptr;
      if (rflag)
         lptr++;
      else
         lptr--;
      return i;
   }
                                            /* read from tempfile */
   if (!rflag)                              /* go back */
      if (fseek (ftmpdata, (long) (-2 * szl), 1) < 0)
         return -1L;

   if (fread ((char *) &i, szl, 1, ftmpdata) <= 0)
      return -1L;
   return i;
}  /* nextlong */


char
getinput ()
{  char c; if (Bflag)
      return getchar();
   else
   {  if (iptr < isptr || iptr > ieptr)
         return EOF;
      else
      {  c = *iptr;
         iptr++;
         return c;
      }
   }
}


seekinput (n, t)
long n;
{  if (Bflag)
      return fseek (stdin, n, t);
   else
   {  switch (t)
      {  case 0: iptr = isptr + n; break;
         case 1: iptr += n; break;
         case 2: iptr = ieptr + n + 1; break;
      }
      if (iptr < isptr || iptr > ieptr)
         return -1;
      else
         return 0;
   }
}


copyin ()
{  register int c;
   mktemp (tmpin);
   if ((ftmpin = fopen (tmpin, "w+")) == NULL)
   {  fprintf (stderr, "rot: can't create tempfile\n");
      exit (1);
   }
   while ((c = getchar()) != EOF)
      putc (c, ftmpin);
   fclose (ftmpin);
   if (freopen (tmpin, "r", stdin) == NULL)
   {  fprintf (stderr, "rot: can't reopen tempfile\n");
      unlink (tmpin);
      exit (1);
   }

}  /* copyin */


on_irupt (s)
{  signal (SIGINT, SIG_IGN);
   signal (SIGTERM, SIG_IGN);
   signal (SIGHUP, SIG_IGN);
   signal (SIGQUIT, SIG_IGN);
   if (Bflag) unlink (tmpdata);
   if (cpin) unlink (tmpin);
   if (s == SIGSEGV)
      fprintf (stderr, bigmsg, "segmentation violation");
   exit (1);
}
!Funky!Stuff!

