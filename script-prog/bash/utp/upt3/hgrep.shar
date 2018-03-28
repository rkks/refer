Subject:  v16i072:  Highlighting grep filter
Newsgroups: comp.sources.unix
Sender: sources
Approved: rsalz@uunet.UU.NET

Submitted-by: Jef Poskanzer <jef@helios.ee.lbl.gov>
Posting-number: Volume 16, Issue 72
Archive-name: hgrep

[  Jef's two-line summary is 100% on-the-mark.  In these days of huge
   source postings to comp.sources.unix, let's not forget the little
   tools and filters.  --r$  ]

Hgrep is a trivial, but cute, front-end for grep.  It takes the results of
the grep and highlights the word that was searched for.

#! /bin/sh
# This is a shell archive, meaning:
# 1. Remove everything above the #! /bin/sh line.
# 2. Save the resulting text in a file.
# 3. Execute the file with /bin/sh (not csh) to create the files:
#	README
#	Makefile
#	hgrep.c
#	hgrep.1
export PATH; PATH=/bin:$PATH
echo shar: extracting "'README'" '(578 characters)'
if test -f 'README'
then
	echo shar: will not over-write existing file "'README'"
else
sed 's/^X//' << \SHAR_EOF > 'README'
X		              hgrep
X		     Distribution of 31oct87
X
XHgrep is a trivial, but cute, front-end for grep.  It takes the results of
Xthe grep and highlights the word that was searched for.
X
XFiles in this distribution:
X
X    README		this
X    Makefile		guess
X    hgrep.c		source file
X    hgrep.1		manual entry
X
XI've tested this stuff Suns and vaxen, but I'm sure bugs remain.
XFeedback is welcome - send bug reports, enhancements, checks, money
Xorders, etc. to the addresses below.
X
X    Jef Poskanzer
X    jef@rtsg.ee.lbl.gov
X    {ucbvax, lll-crg, sun!pacbell, apple, hplabs}!well!pokey
SHAR_EOF
if test 578 -ne "`wc -c < 'README'`"
then
	echo shar: error transmitting "'README'" '(should have been 578 characters)'
fi
fi # end of overwriting check
echo shar: extracting "'Makefile'" '(319 characters)'
if test -f 'Makefile'
then
	echo shar: will not over-write existing file "'Makefile'"
else
sed 's/^X//' << \SHAR_EOF > 'Makefile'
X# Makefile for hgrep
X
Xall:		hgrep hgrep.cat
X
Xhgrep:		hgrep.c
X	cc -O -s hgrep.c -ltermcap -o hgrep
X
Xhgrep.cat:	hgrep.1
X	nroff -man hgrep.1 > hgrep.cat
X
Xclean:
X	-rm -f hgrep hgrep.cat hgrep.shar
X
Xshar:		hgrep.shar
X
Xhgrep.shar:	README Makefile hgrep.c hgrep.1
X	shar -v -c -p X README Makefile hgrep.c hgrep.1 > hgrep.shar
SHAR_EOF
if test 319 -ne "`wc -c < 'Makefile'`"
then
	echo shar: error transmitting "'Makefile'" '(should have been 319 characters)'
fi
fi # end of overwriting check
echo shar: extracting "'hgrep.c'" '(3599 characters)'
if test -f 'hgrep.c'
then
	echo shar: will not over-write existing file "'hgrep.c'"
else
sed 's/^X//' << \SHAR_EOF > 'hgrep.c'
X/*
X** hgrep - a front end to grep that highlights the string found
X**
X** version of 23oct88
X**
X** Copyright (C) 1988 by Jef Poskanzer.
X**
X** Permission to use, copy, modify, and distribute this software and its
X** documentation for any purpose and without fee is hereby granted, provided
X** that the above copyright notice appear in all copies and that both that
X** copyright notice and this permission notice appear in supporting
X** documentation.  This software is provided "as is" without express or
X** implied warranty.
X*/
X
X#include <stdio.h>
X#include <string.h>
X
X#define TBUFSIZE 1024
X#define BIGBUFSIZE 20000
X
Xextern char *getenv();
Xextern char *tgetstr();
Xvoid putch();
X
Xmain( argc, argv )
Xint argc;
Xchar *argv[];
X    {
X    char *term, *strptr, *soptr, *septr;
X    int dumb = 0;
X    int iflag = 0;
X    char buf[TBUFSIZE];
X    static char strbuf[TBUFSIZE];
X    char bigbuf[BIGBUFSIZE];
X    char *grepword = NULL;
X    FILE *grepstream;
X    int i, j, soon, somap[BIGBUFSIZE], grepwordlen;
X
X    /* Initialize termcap stuff. */
X    if ( ! isatty( fileno( stdout ) ) )
X	dumb = 1;
X    else
X	{
X	term = getenv( "TERM" );
X	if ( term == 0 )
X	    dumb = 1;
X	else if ( tgetent( buf, term ) <= 0 )
X	    dumb = 1;
X	else
X	    {
X	    strptr = strbuf;
X	    soptr = tgetstr( "so", &strptr );
X	    septr = tgetstr( "se", &strptr );
X	    if ( soptr == NULL || septr == NULL )
X		dumb = 1;
X	    }
X	}
X
X    /* Construct grep command. */
X    strcpy( bigbuf, "grep" );
X    for ( i = 1; i < argc; i++ )
X	{
X	strcat( bigbuf, " " );
X	/* (Should do some kind of quoting here.) */
X	strcat( bigbuf, argv[i] );
X
X	/* Check for -i flag. */
X	if ( argv[i][0] == '-' && strchr( argv[i], 'i' ) != NULL )
X	    iflag = 1;
X
X	/* Save pointer to word we are grepping for - first non-flag arg. */
X	if ( grepword == NULL && argv[i][0] != '-' )
X	    {
X	    grepword = argv[i];
X	    grepwordlen = strlen( grepword );
X	    }
X	}
X
X    /* Spawn a grep. */
X    grepstream = popen( bigbuf, "r" );
X    if ( grepstream == NULL )
X	{
X	fprintf( stderr, "%s: can't spawn grep\n", argv[0] );
X	exit( 1 );
X	}
X
X    /* Now read and handle results of grep. */
X    soon = 0;
X    while ( fgets( bigbuf, BIGBUFSIZE, grepstream ) != NULL )
X	{
X	if ( dumb )
X	    fputs( bigbuf, stdout );
X	else
X	    {
X	    /* Figure out what to highlight.  This is fairly inefficient,
X	    ** but since we are operating on the output of grep and not
X	    ** the input, it's ok. */
X	    for ( i = 0; bigbuf[i] != '\0'; i++ )
X		somap[i] = 0;
X	    for ( i = 0; bigbuf[i] != '\0'; i++ )
X		if ( iflag )
X		    {
X		    if ( cistrncmp( &(bigbuf[i]), grepword, grepwordlen ) == 0 )
X			for ( j = i; j < i + grepwordlen; j++ )
X			    somap[j] = 1;
X		    }
X		else
X		    {
X		    if ( strncmp( &(bigbuf[i]), grepword, grepwordlen ) == 0 )
X			for ( j = i; j < i + grepwordlen; j++ )
X			    somap[j] = 1;
X		    }
X
X	    /* And now do the highlighting. */
X	    for ( i = 0; bigbuf[i] != '\0'; i++ )
X		{
X		if ( somap[i] )
X		    {
X		    if ( ! soon )
X			{
X			tputs( soptr, 1, putch );
X			soon = 1;
X			}
X		    }
X		else
X		    {
X		    if ( soon )
X			{
X			tputs( septr, 1, putch );
X			soon = 0;
X			}
X		    }
X		putchar( bigbuf[i] );
X		}
X	    }
X	}
X
X    /* Bye. */
X    if ( soon )
X	tputs( septr, 1, putch );
X    exit( pclose( grepstream ) );
X    }
X
Xvoid
Xputch( ch )
Xchar ch;
X    {
X    putchar( ch );
X    }
X
X
X/* cistrncmp - case-insensitive version of strncmp */
X
X#include <ctype.h>
X
Xcistrncmp( s1, s2, n )
Xchar *s1, *s2;
Xint n;
X    {
X    while ( --n >= 0 && (isupper(*s1)?tolower(*s1):*s1) == (isupper(*s2)?tolower(*s2++):*s2++) )
X	if ( *s1++ == '\0' )
X	    return 0;
X    return n < 0 ? 0 : *s1 - *--s2;
X    }
SHAR_EOF
if test 3599 -ne "`wc -c < 'hgrep.c'`"
then
	echo shar: error transmitting "'hgrep.c'" '(should have been 3599 characters)'
fi
fi # end of overwriting check
echo shar: extracting "'hgrep.1'" '(348 characters)'
if test -f 'hgrep.1'
then
	echo shar: will not over-write existing file "'hgrep.1'"
else
sed 's/^X//' << \SHAR_EOF > 'hgrep.1'
X.TH HGREP 1 "23 October 1988"
X.SH NAME
Xhgrep - highlight results of a grep
X.SH SYNOPSIS
Xhgrep <grep args>
X.SH DESCRIPTION
X.I Hgrep
Xis a trivial, but cute, front-end for
X.I grep.
XIt takes the results of the grep and highlights the word that was
Xsearched for.
X.SH "SEE ALSO"
Xgrep(1)
X.SH BUGS
XMeta-characters are not handled.  Quoting is not handled.
SHAR_EOF
if test 348 -ne "`wc -c < 'hgrep.1'`"
then
	echo shar: error transmitting "'hgrep.1'" '(should have been 348 characters)'
fi
fi # end of overwriting check
#	End of shell archive
exit 0

