package com.wickedcooljava.sci;

import cern.colt.bitvector.BitVector;

/*
Example code from Wicked Cool Java (No Starch Press)
Copyright (C) 2005 Brian D. Eubanks

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Note: The LGPL licence can be found online at http://www.gnu.org

*/
/**
 * Basic bit manipulation using the BitVector class from Colt.
 */
public class BasicBitVector {

	public static void main(String[] args) {
		BitVector vec1000 = new BitVector(1000); // size = 1000 bits
		// initially the bits are all set to 0 (false)
		vec1000.set(378); // set bit 378 to 1 (true)
		System.out.println(vec1000.get(378)); // prints "true"
		vec1000.replaceFromToWith(1, 40, true); // sets bits 1-40 to true
		// get the bits from 38 to 50 (inclusive)
		BitVector portion = vec1000.partFromTo(38, 50);
		// get the long value of bits 3 to 10 of the portion BitVector
		long longValue = portion.getLongFromTo(3, 10);
	}
}
