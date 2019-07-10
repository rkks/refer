
package com.wickedcooljava.sci;

import org.jscience.physics.quantities.Constants;
import org.jscience.physics.quantities.Velocity;
import org.jscience.physics.units.NonSI;
import org.jscience.physics.units.Unit;

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
 * Demonstrates the creation of new JScience units, and displaying 
 * physical quantities using them.
 */
public class FurlongFun {

	public static void main(String[] args) {
		// A furlong is an eighth mile
		Unit furlong = NonSI.MILE.multiply(0.125);
		furlong.alias("furlong");
		furlong.label("furlong");
		Velocity c = Constants.c;
		//	A fortnight is an old English unit of 14 days
		Unit fortnight = NonSI.DAY.multiply(14);
		fortnight.alias("fortnight");
		fortnight.label("fortnight");
		Unit furlongperfortnight = furlong.divide(fortnight);
		System.out.println(c.toText(furlongperfortnight));
	}
}
