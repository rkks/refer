package com.wickedcooljava.xml;

import java.io.FileWriter;
import java.io.IOException;
import java.util.Iterator;
import java.util.List;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.HTMLWriter;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.SAXReader;

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
 * This class demonstrates using DOM4J to read XML data from one document,
 * and use it to create an HTML document from the data.
 */
public class DOM4JPurchaseOrderHTML
{

    public static void main(String[] args) 
    throws DocumentException, IOException {
        // Read the input document (po)
        String url = "C:/projects/wcj3/purchaseOrder.xml";
        SAXReader reader = new SAXReader();
        Document po = reader.read(url);
        // Create the output HTML document object
        Document htmlDoc = DocumentHelper.createDocument();
        // Add a title with the extracted Buyer ID
        String buyerID = po.valueOf("/Order/BuyersID");
        String title = "Purchase Order #" + buyerID;
        // Create the root HTML element and document title 
        Element html = htmlDoc.addElement("html");
        html.addElement("head").addElement("title").setText(title);
        // Create a body tag and H1 heading for a title
        Element body = html.addElement("body");
        body.addElement("h1").addText(title);
        // Create the HTML table, with border 
        Element table = body.addElement("table")
                              .addAttribute("border", "1");
        // Create a header row
        Element header = table.addElement("tr");
        header.addElement("th").addText("Quantity");
        header.addElement("th").addText("Description");
        header.addElement("th").addText("Price");
        header.addElement("th").addText("Subtotal");
        // Get the list of line items in the document
        List items = po.selectNodes("/Order/OrderLine");
        Iterator iter = items.iterator();
        while (iter.hasNext())
        {
            Element item = (Element) iter.next();
            // Add a table row
            Element row = table.addElement("tr");
            // Add a cell containing the quantity
            row.addElement("td")
                 .addText(item.valueOf("LineItem/Quantity"))
                 .addAttribute("align","right");
            // Add a cell containing the description
            row.addElement("td")
                 .addText(item.valueOf("LineItem/Item/Description"));
            // Add a cell containing the unit price
            String priceAmount = 
                 item.valueOf("LineItem/Item/BasePrice/PriceAmount");
            row.addElement("td")
                 .addText(priceAmount)
                 .addAttribute("align","right");
            // Add a cell containing the subtotal
            row.addElement("td")
                 .addText(item.valueOf("LineItem/LineExtensionAmount"))
                 .addAttribute("align","right");
        }
        // Produce the total row
        Element totalRow = table.addElement("tr");
        totalRow.addElement("th")
                   .addAttribute("colspan", "3")
                     .addText("Total");
        String total = po.valueOf("/Order/LineExtensionTotalAmount");
        totalRow.addElement("th")
                   .addText(total);
        // Stream the output HTML document to disk
        // We use "pretty printing" to produce easy-to-read HTML
        // Without this, tags have no indentation or whitespace
        OutputFormat format = OutputFormat.createPrettyPrint();
        FileWriter file = new FileWriter("order.html");
        HTMLWriter writer = new HTMLWriter(file, format);
        writer.write(htmlDoc);
        writer.close();
    }
}
