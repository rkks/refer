/*
 *  A usefull macro for assert-like condition checking
 *
 *  Copyright (C) 2008  RIEGL Research ForschungsGmbH
 *  Copyright (C) 2008  Clifford Wolf <clifford@clifford.at>
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
 *
 *  The 2nd argument to the CHECK() macro is the right hand of a
 *  relational test. The macro aborts the program flow if this
 *  test fails. Otherwise the value of the checked expression is
 *  returned. Usage example:
 *
 *	int fd = CHECK(open("/etc/passwd", O_RDONLY), >= 0);
 *
 *  E.g. for checking ranges the value of the to be checked
 *  expression is also available under the name _R inside the
 *  2nd argument of CHECK(). Example given:
 *
 *	int val = CHECK(should_return_0_to_100(), >= 0 && _R <= 100);
 *  
 */

#ifndef CHECK_H
#define CHECK_H 1

/**** BEGIN: http://svn.clifford.at/tools/trunk/examples/check.h ****/

// This is to not confuse the VIM syntax highlighting
#define CHECK_VAL_OPEN (
#define CHECK_VAL_CLOSE )

#define CHECK(result, check)                                         \
   CHECK_VAL_OPEN{                                                   \
     typeof(result) _R = (result);                                   \
     if (!(_R check)) {                                              \
       fprintf(stderr, "Error from '%s' (%d %s) in %s:%d.\n",        \
               #result, (int)_R, #check, __FILE__, __LINE__);        \
       fprintf(stderr, "ERRNO(%d): %s\n", errno, strerror(errno));   \
       abort();                                                      \
     }                                                               \
     _R;                                                             \
   }CHECK_VAL_CLOSE

/**** END: http://svn.clifford.at/tools/trunk/examples/check.h ****/

#endif /* CHECK_H */

