package com.wickedcooljava.strings;

import java.io.FileReader;
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
 * This class demonstrates using a regex pattern to find URLs in a document.
 */
public class URLFinder
{

    public static void main(String[] args)
    throws Exception
    {
        String urlString = 
            "(http|https|ftp)://([a-zA-Z0-9-\\.]+)[/\\w\\.\\-\\+\\?%=&;:,#]*";
        Pattern urlPattern = Pattern.compile(urlString);
        // get the data (somehow)
        String data = getStringData();
        // get a matcher for the data
        Matcher urlMatcher = urlPattern.matcher(data);
        // iterate through the matches
        while (urlMatcher.find())
        {
           int startIndex = urlMatcher.start();
           int endIndex = urlMatcher.end();
           String currentMatch = data.substring(startIndex, endIndex);
           System.out.println(currentMatch);
        }
    }
    
    public static String getStringData()
    {
        char[] buffer = new char[50000];
        int count = 0;
        try
        {
            FileReader fr = new FileReader("C:/projects/regex/data1.txt");
            count = fr.read(buffer);
            fr.close();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
        return new String(buffer, 0, count);
        
    }
    
}
