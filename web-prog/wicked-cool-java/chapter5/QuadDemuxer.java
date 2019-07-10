
package com.wickedcooljava.sci;

import cern.colt.bitvector.BitVector;
import net.sf.jga.fn.UnaryFunctor;

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
 * This class demonstrates creating a 4-bit multiplexer using a BitVector
 * functor.
 */
public class QuadDemuxer extends UnaryFunctor<BitVector, BitVector> {
	public BitVector fn(BitVector in) {
		// a 4-bit output vector, defaults to all 0
		BitVector outVec = new BitVector(4);
		// get the input data
		boolean data = in.get(0);
		if (data) {
			// get the selector bits as an int
			int selector = (int) in.getLongFromTo(1, 2);
			outVec.set(selector);
		}
		return outVec;
	}
}
