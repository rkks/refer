package com.wickedcooljava.graphics;

import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import javax.swing.JFrame;

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
 * Simple affine transformation
 */
public class MyTransform extends JFrame {

	public MyTransform(String title) {
		super(title);
	}
	
	public void paint(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		g2d.shear(0.5,0.5);
		g2d.setFont(Font.decode("arial-bold-36"));
		g2d.drawString("Affinity", 100, 100);
		g2d.drawOval(100, 150, 35, 50);
		g2d.drawOval(135, 150, 35, 50);
	}
	
	public static void main(String[] args) {
		MyTransform aft = new MyTransform("Affinity");
		aft.setSize(300,300);
		aft.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		aft.setVisible(true);
	}
}

