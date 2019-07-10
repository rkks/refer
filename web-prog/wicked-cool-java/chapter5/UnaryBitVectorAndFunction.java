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
 * This functor performs an AND function on the bits in a BitVector,
 * and returns a single-bit BitVector.
 */
public class UnaryBitVectorAndFunction 
extends	UnaryFunctor<BitVector, BitVector> {
	public BitVector fn(BitVector in) {
		int oneBits = in.cardinality(); // how many bits are 1
		int size = in.size(); // the vector size
		BitVector outVec = new BitVector(1); // a one-bit output
		outVec.put(0, size == oneBits); // AND = all ones
		return outVec;
	}
	
}
