package com.wickedcooljava.graphics;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.awt.image.WritableRaster;
import java.util.Random;

import javax.swing.JFrame;
import javax.swing.JPanel;

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
 * Simple 2D data visualization
 */
public class BinaryPlotPanel extends JPanel {

	BufferedImage buffIm;

	private byte[] buffer;

	public BinaryPlotPanel(int x, int y) {
		buffIm = new BufferedImage(x, y, BufferedImage.TYPE_BYTE_BINARY);
		WritableRaster rasta = buffIm.getRaster();
		DataBufferByte buf = (DataBufferByte) rasta.getDataBuffer();
		buffer = buf.getData();
	}

	public int getImageWidth() {
		return buffIm.getWidth();
	}

	public int getImageHeight() {
		return buffIm.getHeight();
	}

	public byte[] getBuffer() {
		return buffer;
	}

	public Graphics2D getBufferGraphics() {
		return buffIm.createGraphics();
	}

	public void paint(Graphics arg0) {
		Graphics2D g = (Graphics2D) arg0;
		g.drawImage(buffIm, 0, 0, this);
	}

	public static void main(String[] args) {
		int x = 600;
		int y = 400;
		BinaryPlotPanel bpp = new BinaryPlotPanel(x, y);
		byte[] data = bpp.getBuffer();
		Random r = new Random();
		// fill the buffer with random data
		r.nextBytes(data);
		// print "Wicked Cool Java"
		Graphics2D g = bpp.getBufferGraphics();
		g.setFont(Font.decode("Courier-48"));
		g.setColor(Color.BLACK);
		g.drawString("Wicked Cool Java", 80, 200);
		JFrame jf = new JFrame("Random " + x + "x" + y);
		jf.setSize(x + 10, y + 30);
		jf.add(bpp);
		jf.setVisible(true);
	}

}
