package com.wickedcooljava.graphics;

import java.io.File;
import java.io.IOException;

import javax.swing.JFrame;

import org.apache.batik.swing.JSVGCanvas;
import org.apache.batik.swing.gvt.GVTTreeRendererAdapter;
import org.apache.batik.swing.gvt.GVTTreeRendererEvent;
import org.apache.batik.swing.svg.GVTTreeBuilderAdapter;
import org.apache.batik.swing.svg.GVTTreeBuilderEvent;
import org.apache.batik.swing.svg.SVGDocumentLoaderAdapter;
import org.apache.batik.swing.svg.SVGDocumentLoaderEvent;

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
 * Displaying SVG files using Batik.
 */
public class BatikDisplay {

	public static void main(String[] args) 
	throws IOException {

        //C:/projects/batik-1.5.1/samples/sizeOfSun.svg
        //File file = new File("C:/projects/batik-1.5.1/samples/sizeOfSun.svg");
        File file = new File("test.svg");
        String url = file.toURL().toString();
        System.out.println(url);
        JSVGCanvas canvas = new JSVGCanvas();
        canvas.setEnableImageZoomInteractor(true);
        canvas.setSize(500,500);
        // Set the JSVGCanvas listeners.
        canvas.addSVGDocumentLoaderListener(new SVGDocumentLoaderAdapter() {
            public void documentLoadingStarted(SVGDocumentLoaderEvent e) {
            	System.out.println("Document Loading...");
            }
            public void documentLoadingCompleted(SVGDocumentLoaderEvent e) {
            	System.out.println("Document Loaded.");
            }
        });
        canvas.addGVTTreeBuilderListener(new GVTTreeBuilderAdapter() {
            public void gvtBuildStarted(GVTTreeBuilderEvent e) {
                System.out.println("Build Started...");
            }
            public void gvtBuildCompleted(GVTTreeBuilderEvent e) {
            	System.out.println("Build Done.");
            }
        });
        canvas.addGVTTreeRendererListener(new GVTTreeRendererAdapter() {
            public void gvtRenderingPrepare(GVTTreeRendererEvent e) {
            	System.out.println("Rendering Started...");
            }
            public void gvtRenderingCompleted(GVTTreeRendererEvent e) {
            	System.out.println("Rendering Done.");
            }
        });
        canvas.setURI(url);
        
        JFrame f = new JFrame("Batik Graphics");
        f.getContentPane().add(canvas);
        f.pack();
        f.setVisible(true);
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

//        JFileChooser fc = new JFileChooser(".");
//        int choice = fc.showOpenDialog(f);
//        if (choice == JFileChooser.APPROVE_OPTION) {
//            File file = fc.getSelectedFile();
//            try {
//                canvas.setURI(file.toURL().toString());
//            } catch (IOException ex) {
//                ex.printStackTrace();
//            }
//        }

	}
}
