
package com.wickedcooljava.sci;

import org.jscience.mathematics.numbers.Rational;

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
 * Example showing how to use Rational object from JScience.
 */
public class RationalExample {

	public static void main(String[] args) {
		Rational oneThird = Rational.valueOf("1/3");
		Rational nine87654321 = Rational.valueOf("987654321/1");
		Rational msixteen = Rational.valueOf("-16/1");
		Rational msixteenOver987654321 = msixteen.divide(nine87654321);
		Rational aNumber = oneThird.multiply(msixteenOver987654321);
	}
}
