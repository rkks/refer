package com.wickedcooljava.xml;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

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
 * This class removes all tags from an XML document, and places any text
 * into a buffer.
 */
public class TagStripper extends DefaultHandler
{

	StringBuffer buffer = new StringBuffer();
	
	public void startElement(String uri, String localName, String qName,
			Attributes attributes) throws SAXException {
		buffer.append(' ');
	}
	
	public void characters(char[] ch, int start, int length)
			throws SAXException {
		buffer.append(ch, start, length);
	}
	
	public void endElement(String uri, String localName, String qName)
			throws SAXException {
	    buffer.append(' ');
	}
	
	public String getBuffer() {
		return buffer.toString();
	}
	
}
