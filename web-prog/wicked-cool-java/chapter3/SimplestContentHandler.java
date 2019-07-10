package com.wickedcooljava.xml;

import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.SAXException;
import org.xml.sax.Attributes;

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
This class loads an XML document into a SAX ContentHandler
*/
public class SimplestContentHandler extends DefaultHandler {
  public void startElement(String uri, String localName,
                           String qName, Attributes attributes) 
  throws SAXException {
      System.out.println("Opening tag " + localName + "");
  }

  public void characters(char[] ch, int start, int length)
  throws SAXException {
     String text = new String(ch, start, length);
     System.out.print(text);
  }

  public void endElement(String uri, String localName, String qName)
  throws SAXException {
      System.out.println("Closing tag " + localName);
  }
}