package com.wickedcooljava.graphics;

import java.awt.Color;
import java.util.Random;
import edu.umd.cs.piccolo.nodes.PPath;
import edu.umd.cs.piccolox.PFrame;

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
 * Example creating a zoomable application using Piccolo
 */
public class PiccoloExample extends PFrame {

	public void initialize() {
		setTitle("Room To Zoom");
		Random random = new Random();
		PPath path = new PPath();
		path.moveTo(50, 50);
		for (int i = 0; i < 20; i++) {
			path.lineTo(random.nextInt(200), random.nextInt(200));
		}
		path.closePath();
		path.setPaint(Color.blue);
		getCanvas().getLayer().addChild(path);
	}

	public static void main(String[] args) {
		new PiccoloExample();
	}
}
