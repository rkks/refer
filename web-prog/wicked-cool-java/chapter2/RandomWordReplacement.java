package com.wickedcooljava.strings;

import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

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
 * This class demonstrates using regex to replace words.
 */
public class RandomWordReplacement
{

    public static void main(String[] args)
    {
        StringBuffer result = new StringBuffer();
        String[] wordChoices = new String[] 
           {"repetition", "duplication", "copying", 
           "reiteration", "recurrence", "redundance"};
        Random rand = new Random();

        String data = getStringData();
        Pattern repPattern = Pattern.compile("(\\s)(repetition)([\\s;\\.,])");
        Matcher repMatcher = repPattern.matcher(data);
        while (repMatcher.find())
        {
           int wordIndex = rand.nextInt(wordChoices.length);
           String replacement = "$1" + wordChoices[wordIndex] + "$3";
           repMatcher.appendReplacement(result, replacement);
        }
        // append the last portion (without any matches)
        repMatcher.appendTail(result);
        System.out.println(result);
        
    }
    
    private static String getStringData()
    {
       return "The repetition of this data is obviously a repetition, \n"
       + "and those who don't like repetition will have to accept this \n"
       + "repetition anyway. I know it's a repetition to say that, but \n"
       + "repetition is what is this is all about. And I don't think it's \n"
       + "a repetition to say that!"
       ;
    }
    
}
