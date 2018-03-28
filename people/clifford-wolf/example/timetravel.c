/*
 *  Timetravel Preload Library - add an offset to time() results
 *
 *  Copyright (C) 2001  Clifford Wolf <clifford@clifford.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *  USA
 *  
 *  gcc -Wall -O2 -ldl --shared -o timetravel.so timetravel.c
 */

#define _GNU_SOURCE
#define _REENTRANT

#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>

extern time_t time(time_t *t);

time_t time(time_t *t)
{
	int (*orig_time)(time_t *);
	time_t returncode;
	
	orig_time = dlsym(RTLD_NEXT,"time");
	returncode = orig_time(NULL) - (24*60*60*10);

	if (t != NULL) *t = returncode;
	return returncode;
}

