/*
**	$Header: grabchars.h,v 1.9 89/12/29 21:15:25 daniel grabchars_1_9 $
**
**	grabchars.h - setup for grabchars
**
**	Dan Smith (daniel@island.uu.net), November 29, 1988
*/

#define mk_lower(c) (isupper (c) ? tolower (c) : c)
#define mk_upper(c) (islower (c) ? toupper (c) : c)

/* define SIGRET=void in Makefile for Sun 4.X */
#ifndef SIGRET
#define SIGRET int
#endif

struct flag_type {
	int both;	/* output to stdout and stderr */
	int check;	/* filter input */
	int dflt;	/* use following char or string as default */
	int used_dflt;	/* if editing on and used default don't echo stderr */
	int flush;	/* if set flush input buffer */
	int ret_key;	/* RETURN key exits when set */
	int silent;	/* be quiet, just return a status */
	int erase;	/* DV: honor erase/kill characters */
	int lower;	/* map chars to lower case */
	int upper;	/* map chars to upper case */
};

typedef struct flag_type FLAG;

