/*
 *  Some usefull macros for tracing program flow
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 *  The PING() and PING_LIMIT() macros do only use putc() and puts() and
 *  do not depend on a printf function.
 */

#ifndef PING_H
#define PING_H 1

#define _PING_N2(_d) #_d
#define _PING_N1(_d) _PING_N2(_d)
#define PING_RAW(_lim, _cmd) do {                                        \
        static unsigned int _ping_count = 0;                             \
        if (++_ping_count > _lim) {                                      \
                int _ping_i;                                             \
                puts("PING[");                                           \
                for (_ping_i=7; _ping_i>=0; _ping_i--)                   \
                        putc("0123456789abcdef"[                         \
                                        (_ping_count>>(_ping_i*4))&15]); \
                puts("]: " __FILE__ ":" _PING_N1(__LINE__) " ");         \
                puts(__PRETTY_FUNCTION__); puts(": ");                   \
                _cmd; puts("\n");                                        \
        } } while (0)
#define PINGF_LIMIT(_lim, _fmt_args...) PING_RAW(_lim, printf(_fmt_args))
#define PING_LIMIT(_lim, _s) PING_RAW(_lim, puts(_s))
#define PINGF(_fmt_args...) PING_RAW(0, printf(_fmt_args))
#define PING(_s) PING_RAW(0, puts(_s))

#endif /* PING_H */

