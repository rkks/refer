package com.wickedcooljava.xml;

import java.util.HashMap;
import java.util.Stack;
import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.helpers.XMLReaderFactory;
import org.xml.sax.ContentHandler;
import org.xml.sax.SAXException;
import org.xml.sax.Attributes;
import org.xml.sax.XMLReader;

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
 * This class demonstrates using a Stack and HashMap as data structures
 * for use with SAX.
 */
public class LineItemReportHandler extends DefaultHandler {

  private Stack path = new Stack();
  private HashMap values = new HashMap();
  
  public void startElement(String uri, String localName,
                           String qName, Attributes attributes) 
  throws SAXException {
    path.push(localName);
    values.put(localName, "");
  }

  public void characters(char[] ch, int start, int length)
  throws SAXException {
     String text = new String(ch, start, length);
     // what is the most recent tag?
     String currentTag = (String) path.peek();
     // what is the value of the text within it so far?
     String currentValue = (String) values.get(currentTag);
     // append the text and add it to the map
     currentValue = currentValue + text;
     values.put(currentTag, currentValue);
  }

  public void endElement(String uri, String localName,  
                         String qName)
  throws SAXException {
     path.pop();
     if (localName.equals("LineItem")) {
         String amount = (String) values.get("LineExtensionAmount");
         if (Double.parseDouble(amount) >= 30.0) {
             System.out.print("Quantity ");
             System.out.print(values.get("Quantity"));
             System.out.print(" of \"");
             System.out.print(values.get("Description"));
             System.out.print("\" costs $");
             System.out.println(amount);
         }
     }
  }
  
  public static void main(String[] args)
  throws Exception
  {
      XMLReader parser = XMLReaderFactory.createXMLReader();
      ContentHandler handler = new LineItemReportHandler();
      parser.setContentHandler(handler);
      String url = "http://docs.oasis-open.org/ubl/cd-UBL-1.0/" +
                   "xml/office/UBL-Order-1.0-Office-Example.xml";
      parser.parse(url);
  }
}

