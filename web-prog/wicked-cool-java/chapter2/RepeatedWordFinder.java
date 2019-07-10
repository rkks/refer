package com.wickedcooljava.strings;

import java.io.FileReader;
import java.io.IOException;
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
 * This class finds words that are repeated (i.e. of of, the the).
 */
public class RepeatedWordFinder
{
    public static void main(String[] args) throws IOException
    
    {
    	// replace with your document of choice
        FileReader reader = new FileReader("C:/document.txt");
        char[] buf = new char[100000];
        int count = reader.read(buf);
        String data = new String(buf, 0, count);
        
        String wordPattern = "\\s(of|or|the|to)\\s+\\1[\\s\\.,;]";
        Pattern wPattern = Pattern.compile(wordPattern, Pattern.CASE_INSENSITIVE);
        Matcher wMatcher = wPattern.matcher(data);
        while (wMatcher.find())
        {
            int start = wMatcher.start();
            int end = wMatcher.end();
            String match = data.substring(start, end);
            System.out.println(match);
        }
        
    }
}
