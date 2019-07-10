package com.wickedcooljava.media;

import java.util.Random;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.SourceDataLine;

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
 * Working with low-level Java Sound to output sound.
 */
public class LineInfoTest {

	public static void main(String[] args) {
		try {
			AudioFormat format = new AudioFormat(44100, 16, 2, true, false);
			SourceDataLine line = AudioSystem.getSourceDataLine(format);
			line.open();
			line.start();
			// now you may write data to the line
			// here we create some random data
			int size = line.getBufferSize();
			byte[] data = new byte[size];
			Random random = new Random();
			long start = System.currentTimeMillis();
			for (int i = 0; i < 10; i++) {
				random.nextBytes(data);
				line.write(data, 0, size);
			}
			// when you are done, drain and close the line
			line.drain();
			line.close();
		} catch (LineUnavailableException e) {
			e.printStackTrace();
		}

	}
}
