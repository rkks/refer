
package com.wickedcooljava.xml;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.XMLFilterImpl;

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
 * This class implements an XML filter that ignores anything within 
 * &lt;ignore&gt; tags. Note that it works at only one level of 
 * ignore tag (not nested).
 */
public class IgnoringFilter extends XMLFilterImpl
{
	/** Whether we are currently in ignore mode. */
    boolean ignoring = false;
    
    public void startElement(String url, String localName, String qName,
            Attributes att) throws SAXException {
        if (localName.equals("ignore"))
        {
            ignoring = true;
        }
        if (!ignoring) {
           super.startElement(url, localName, qName, att);
        }
    }
    
    public void characters(char[] data, int start, int length)
    throws SAXException
    {
        if (!ignoring)
        {
            super.characters(data, start, length);
        }
    }
    
    public void endElement(String url, String localName, String qName)
    throws SAXException {
        if (localName.equals("ignore")) {
            ignoring = false;
        }
        else if (!ignoring) {
            super.endElement(url, localName, qName);
        }
    }
}
