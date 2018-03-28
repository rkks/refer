/*
$Id: util.h,v 1.6 2009/01/12 02:45:26 alexsisson Exp $

(C) Copyright 2002-2003 Alex Sisson (alexsisson@gmail.com)

This file is part of shed.

shed is free software; you can redistribute it and/or modify
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

shed is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with shed; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <getopt.h>
#include <stdint.h>

/* defines */
#define strequ(s1,s2) (!(strcmp(s1,s2)))

/* ascii control char descs */
extern char  ascii_short_desc[];
extern char *ascii_long_desc[];

/* functions */
char *getascii(unsigned char c, char *s, int mode);
char *getstring(uint64_t n, char *s, int base, int width);
int64_t parsestring(char *s, int base);
int calcwidth(uint64_t n, int base);
char *getopt_makeoptstring(struct option *opt);

#endif
