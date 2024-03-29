
package com.wickedcooljava.misc;

/*
 * Example code from Wicked Cool Java (No Starch Press) Copyright (C) 2005 Brian
 * D. Eubanks
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 * 
 * Note: The LGPL licence can be found online at http://www.gnu.org
 * 
 */
/**
 * Simple Lejos example
 */
public class LejosExample {

	public static void main(String[] args) {
		// set the motor power levels
		Motor.A.setPower(5);
		Motor.B.setPower(5);
		// move motor A forward
		Motor.A.forward();
		// sleep for 500 ms
		Thread.sleep(500);
		// move the motors
		Motor.B.forward();
		Motor.A.backward();
		// sleep for 500 ms
		Thread.sleep(500);
		// stop the motors
		Motor.A.stop();
		Motor.B.stop();
	}

}
