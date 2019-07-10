
package com.wickedcooljava.sci;

import org.jscience.mathematics.functions.Constant;
import org.jscience.mathematics.functions.Function;
import org.jscience.mathematics.functions.Polynomial;
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
 * Example showing the use of Polynomial objects in JScience.
 */
public class PolynomialExample {

	public static void main(String[] args) {
		Function.Variable x = Function.Variable.X;
		Function.Variable y = Function.Variable.Y;
		Polynomial xpoly = Polynomial.valueOf(Rational.ONE, x);
		Polynomial ypoly = Polynomial.valueOf(Rational.ONE, y);
		Rational nine = Rational.valueOf("9/1");
		Rational sixty = Rational.valueOf("60/1");
		Rational seven15ths = Rational.valueOf("7/15");

		Polynomial seven15X5 = 
			Polynomial.valueOf(seven15ths, Polynomial.Term.valueOf(x, 5));

		Polynomial nineXY = (Polynomial) 
			Constant.valueOf(nine).times(xpoly).times(ypoly);

		Polynomial poly = (Polynomial) 
			seven15X5.plus(nineXY).plus(Constant.valueOf(sixty));

		Function.Variable.X.setValue(Rational.valueOf("5/7"));
		Function.Variable.Y.setValue(Rational.ONE);
		System.out.println(poly);
		System.out.println(poly.evaluate());
	}
}
