
package com.wickedcooljava.graphics;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTextField;
import org.swixml.SwingEngine;

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
 * Example to load a SwiXML GUI from an XML description file. 
 */
public class SwixMLTest {

	public JFrame theFrame;
	public JPanel panel1, panel2, panel3;
	public JTextField title, author, subject, publisher;
	public JButton btn1;
	
	public SwixMLTest() {
		try {
			SwingEngine swen = new SwingEngine(this);
			swen.render("bookGUI.xml");
			theFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
			theFrame.setVisible(true);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}
	
	public static void main(String[] args) throws Exception {
		SwixMLTest t = new SwixMLTest();
	}
}
