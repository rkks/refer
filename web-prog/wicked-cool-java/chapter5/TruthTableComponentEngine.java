
package com.wickedcooljava.sci.component;

import com.wickedcooljava.sci.SimpleTruthTable;

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
 * A Boolean Component which operates using a truth table
 */
public class TruthTableComponentEngine implements ComponentEngine<Boolean> {

	private SimpleTruthTable tt;
	
	public TruthTableComponentEngine(SimpleTruthTable table) {
		tt = table;
	}
	
	public void process(PortImpl<Boolean>[] in, PortImpl<Boolean>[] out) {
		int value = toInt(in);
		setPortsFromLong(tt.retrieve(value), out);
	}

	/**
	 * Converts data from port values to int (32 bits)
	 */
	public static int toInt(PortImpl<Boolean>[] data) {
		int value = 0;
		int mask = 1;
		int index = 0;
		int max = data.length;
		while (index < 32 && index < max) {
			if (Boolean.TRUE.equals(data[index].getValue())) {
				value |= mask;
			}
			mask <<= 1;
			index++;
		}
		return value;
	}

	/**
	 * Sets the boolean value of the ports based on each bit of a long int
	 */
	public static void setPortsFromLong(long data, PortImpl<Boolean>[] ports) {
		long mask = 1L;
		int index = 0;
		int max = ports.length;
		while (index < 64 && index < max) {
			ports[index].setValue((data & mask) != 0L);
			index++;
			mask <<= 1;
		}
	}
	
}
