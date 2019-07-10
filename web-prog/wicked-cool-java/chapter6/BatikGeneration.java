package com.wickedcooljava.graphics;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.geom.CubicCurve2D;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.Writer;

import org.apache.batik.dom.svg.SVGDOMImplementation;
import org.apache.batik.svggen.SVGGraphics2D;
import org.w3c.dom.DOMImplementation;
import org.w3c.dom.svg.SVGDocument;

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
 * Example Batik code to create an SVG file.
 */
public class BatikGeneration {

	public static void main(String[] args) throws Exception {
		// Get a DOMImplementation
        DOMImplementation domImpl =
            SVGDOMImplementation.getDOMImplementation();

        // Create a document with the appropriate namespace
        SVGDocument document = 
        	(SVGDocument) domImpl.createDocument(SVGDOMImplementation.SVG_NAMESPACE_URI, "svg", null);

        // Create an instance of the SVG Generator
        SVGGraphics2D graphics = new SVGGraphics2D(document);
        // draw onto the SVG Graphics object
        graphics.setSVGCanvasSize(new Dimension(500,500));
        graphics.setColor(Color.BLACK);
        graphics.drawRect(190,190,60,60);
        graphics.setFont(Font.decode("Courier-bold-20"));
        graphics.drawString("An SVG document created with Batik", 40, 100);
        graphics.setColor(Color.BLUE);
        CubicCurve2D curve = 
        	new CubicCurve2D.Double(150, 150, 175, 125, 300, 175, 300, 300); 
        graphics.draw(curve);
        graphics.drawOval(200,200,40,40);
        graphics.setColor(Color.RED);
        graphics.drawOval(100,180,60,80);

        // Finally, stream out SVG to the standard output using UTF-8
        // character to byte encoding
        boolean useCSS = true; // we want to use CSS style attribute
        Writer out = new OutputStreamWriter(new FileOutputStream("test.svg"), "UTF-8");
        graphics.stream(out, useCSS);
        out.flush();
        out.close();
        System.out.println("wrote file");      
	}
	
}
