/*
 *  Tiny macro for writing 8-bit integer constants as binary numbers
 *
 *  Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
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
 *  E.g. bits2byte(1010, 0011) expands to the dezimal number 163 in a way that
 *  gcc even optimizes the macro away when called with -O0 for no optimization.
 */

#ifndef BITS2BYTE_H
#define BITS2BYTE_H 1

#define bits2byte(_hbits, _lbits) \
   (((((1 ## _hbits) / 1000)&1) << 7) | \
    ((((1 ## _hbits) /  100)&1) << 6) | \
    ((((1 ## _hbits) /   10)&1) << 5) | \
    ((((1 ## _hbits) /    1)&1) << 4) | \
    ((((1 ## _lbits) / 1000)&1) << 3) | \
    ((((1 ## _lbits) /  100)&1) << 2) | \
    ((((1 ## _lbits) /   10)&1) << 1) | \
    ((((1 ## _lbits) /    1)&1) << 0))

#endif /* BITS2BYTE_H */

