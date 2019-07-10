
package com.wickedcooljava.sci;

import cern.colt.bitvector.BitMatrix;

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
 * A simple implementation of a truth table. 
 */
public class SimpleTruthTable {
	/// the backing BitMatrix of truth table bits
	protected BitMatrix truthMatrix;
	/// the number of input bits
	private int inSize;
    /// the number of output bits
	private int outSize;	

	/**
	 * Create a truth table with the specified number of input and output bits.
	 * Initially the output bits are zero for any input.
	 */
	public SimpleTruthTable(int inputs, int outputs) {
		inSize = inputs;
		outSize = outputs;
		int rows = 1 << inputs;
		int cols = outputs;
		truthMatrix = new BitMatrix(cols, rows);
	}

	/**
	 * Returns the number of input bits.
	 */
	public int getInputSize() {
		return inSize;
	}

	/**
	 * Returns the number of output bits.
	 */
	public int getOutputSize() {
		return outSize;
	}
	
	/**
	 * Store an input/output mapping in the truth table.
	 * For the input value <code>inputVal</code>, the output should be <code>out</code>.
	 * Warning: the input value is not checked against the input size.
	 */
	public void store(int inputVal, long out) {
		int start = inputVal * outSize;
		int end = start + outSize - 1; // inclusive
		truthMatrix.toBitVector().putLongFromTo(out, start, end);
	}
	
	/**
	 * Retrieve the output value from the truth table, for the given input value.
	 * Warning: the input value is not checked against the input size.
	 */
	public long retrieve(int inputVal) {
		int start = inputVal * outSize;
		int end = start + outSize - 1; // inclusive
		long out = truthMatrix.toBitVector().getLongFromTo(start, end);
		return out;
	}
	
	/**
	 * Convenience method to make creating truth tables easier.
	 * Sets the value of the BitMatrix (backing store) of the table.
	 */
	public void setTruthMatrix(BitMatrix newMat) {
		if (newMat.rows() != truthMatrix.rows()
				|| newMat.columns() != truthMatrix.columns()) {
			throw new IndexOutOfBoundsException();
		}
		truthMatrix = newMat;
	}
	
	/**
	 * Access to the matrix backing store.
	 */
	public BitMatrix getTruthMatrix() {
		return truthMatrix;
	}
}
