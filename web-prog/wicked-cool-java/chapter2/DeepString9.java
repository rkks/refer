package com.wickedcooljava.strings;

import java.util.Arrays;

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
 * This class demonstrates deep versus shallow printing of an array in Java 5.
 */
public class DeepString9
{

    public static void main(String[] args)
    {
        Object[] multiDimensional = new Object[] {
                "mercury", new Object[] {}, 
                "venus", new Object[] {},
                "earth", new Object[] { "moon" },
                "mars", new Object[] { "phobos", "deimos" }
             };
             System.out.println(multiDimensional);
             System.out.println(Arrays.toString(multiDimensional));
             System.out.println(Arrays.deepToString(multiDimensional));
    }
}
