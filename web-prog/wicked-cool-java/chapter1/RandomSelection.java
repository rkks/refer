package com.wickedcooljava.core;

import java.util.ArrayList;
import java.security.SecureRandom;

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
 * This demonstrates how to create a class that uses generics.
 * It selects an item at random from a list, which it manages.
 * @param <T> the type returned from the list
 */
public class RandomSelection<T>
{
   private ArrayList<T> list;
   private SecureRandom random;

   public RandomSelection()
   {
      list = new ArrayList<T>();
      random = new SecureRandom();
   }

   public void add(T element)
   {
      list.add(element);
   }

   public synchronized T randomElement()
   {
      int randomIndex = random.nextInt(list.size());
      return list.get(randomIndex);
   }

   public static void main(String[] args)
   {
      RandomSelection<String> rs = new RandomSelection<String>();
      for (String s : args)
      {
         rs.add(s);
      }
      System.out.println(rs.randomElement());
      System.out.println(rs.randomElement());
      System.out.println(rs.randomElement());
   }
}
