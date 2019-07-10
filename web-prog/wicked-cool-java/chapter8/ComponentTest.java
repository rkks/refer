package com.wickedcooljava.misc;

import java.util.Date;

import junit.framework.Assert;
import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

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
 * Example JUnit test
 */
public class ComponentTest extends TestCase {
	public void testProcess() {
		// initialize the objects used in the test
		// use the objects, and obtain the result
		String result = (new Date()).toString();
		String expected = "abcdef";
		// check if the result is the correct output value
		Assert.assertTrue(result.equals(expected));
	}

	// you can create other methods to test other functionality
	// create a test suite
	public static Test suite() {
		return new TestSuite(ComponentTest.class);
	}
}