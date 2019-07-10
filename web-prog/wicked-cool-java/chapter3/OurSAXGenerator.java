package com.wickedcooljava.xml;

import org.xml.sax.ContentHandler;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.AttributesImpl;

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
  This class demonstrates how SAX events can be sent to a ContentHandler.
*/
public class OurSAXGenerator {
	private ContentHandler handler; // initialized elsewhere

	/** Sends an open tag to the handler */
	private void open(String tag) throws SAXException {
		handler.startElement("", tag, tag, new AttributesImpl());
	}

	/** Sends a close tag to the handler */
	private void close(String tag) throws SAXException {
		handler.endElement("", tag, tag);
	}

	/** Sends an open/close tag with embedded text content to the handler */
	private void tagText(String tag, String value) throws SAXException {
		open(tag);
		handler.characters(value.toCharArray(), 0, value.length());
		close(tag);
	}

	public OurSAXGenerator() {
		handler = new SimplestContentHandler();
		try {
			handler.startDocument();
			open("OrderCancellation"); // open the OrderCancellation tag
				tagText("ID", "20031654-X"); // ID tag with embedded text
				tagText("IssueDateTime", "2003-03-09T09:30:47");
				tagText("CancellationNote", "order replaced");
				open("OrderReference");
					tagText("BuyersID", "20031654-1");
					tagText("IssueDate", "2003-03-07");
				close("OrderReference");
				open("BuyerParty");
					open("Party");
						open("PartyName");
							tagText("Name", "Bills Microdevices");
						close("PartyName");
					close("Party");
				close("BuyerParty");
				open("SellerParty");
					open("Party");
						open("PartyName");
							tagText("Name", "Joes Office Supply");
						close("PartyName");
					close("Party");
					open("OrderContact");
						tagText("Name", "Betty Jo Beoloski");
					close("OrderContact");
				close("SellerParty");
			close("OrderCancellation"); // close the OrderCancellation tag
			handler.endDocument();
		} catch (SAXException e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) {
		new OurSAXGenerator();
	}
}