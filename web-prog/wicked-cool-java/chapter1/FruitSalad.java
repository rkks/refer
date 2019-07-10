package com.wickedcooljava.core;

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
 * This example shows the use of enum types in Java 5.0 
 */
public class FruitSalad {

		public enum FruitCategory {
			SWEET, CITRUS, SMELLY
		}
	
	    public enum Fruit {
	        APPLE      
	        { FruitCategory getCategory() {return FruitCategory.SWEET;} },
	        ORANGE     
	        { FruitCategory getCategory() {return FruitCategory.CITRUS;} },
	        GRAPEFRUIT 
	        { FruitCategory getCategory() {return FruitCategory.CITRUS;} },
	        BANANA     
	        { FruitCategory getCategory() {return FruitCategory.SWEET;} },
	        DURIAN     
	        { FruitCategory getCategory() {return FruitCategory.SMELLY;} };
	        
	        abstract FruitCategory getCategory();
	    }
	    
	    public static void main (String [] args) {
	       Fruit a = Fruit.APPLE;
	       System.out.println ("The toString() for a: " + a);
	       System.out.println ("a.getCategory() is: " + a.getCategory());
	       
	       for (Fruit f : Fruit.values()) {
	           System.out.println ("Fruit is: " + f);     
	       }
	    }

}
