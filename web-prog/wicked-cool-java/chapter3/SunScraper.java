package com.wickedcooljava.xml;

import java.io.IOException;
import java.io.InputStream;
import java.io.StringReader;
import java.net.URL;
import java.util.Scanner;

import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.XMLReaderFactory;

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
 * This class demonstrates one way of "screen scraping" data from a web page.
 */
public class SunScraper {

	public static void main(String[] args) throws SAXException, IOException {
		XMLReader reader = XMLReaderFactory.createXMLReader();
		TagStripper strip = new TagStripper();
		reader.setContentHandler(strip);
		String all = readFully("http://en.wikipedia.org/wiki/Sun");
		StringReader strRdr = new StringReader(all);
		InputSource src = new InputSource(strRdr);
		reader.parse(src);
		Scanner scanner = new Scanner(strip.getBuffer());
		scanner.findWithinHorizon("Surface temperature", 0);
		String surfTemp = scanner.next();
		String units = scanner.next();
		System.out.println(surfTemp + " " + units);
	}
	
	public static String readFully(String urlString) 
	throws IOException
	{
		StringBuffer result = new StringBuffer();
		byte[] data = new byte[65536];
		URL url = new URL(urlString);
		InputStream in = url.openStream();
		int count = 0;
		while ((count = in.read(data)) > 0)
		{
			String more = new String(data, 0, count);
			result.append(more);
		}
		return result.toString();
	}
	
}
