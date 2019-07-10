package com.wickedcooljava.media;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.SourceDataLine;
import javax.sound.sampled.TargetDataLine;

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
 * Example showing reading from the default input line (usually microphone)
 * then playing it back through the default output line.
 */
public class MicrophoneListener {

	public static void main(String[] args) {
		
		AudioFormat format;
		int size;
		byte[] data;
		try {
			// we'll save recorded data to a file called audio.raw
			FileOutputStream save = new FileOutputStream("audio.raw");
			format = new AudioFormat(22100, 8, 2, true, false);
			TargetDataLine input = AudioSystem.getTargetDataLine(format);
			input.open();
			// get the buffer size 
			size = input.getBufferSize();
			data = new byte[size];
			input.start();
			// loop through 5 buffer lengths
			for (int i=0;i<5;i++) {
				input.read(data, 0, size);
				// process the input data buffer
				// as an example, we show saving the buffer data to a file
				save.write(data);
			}
			input.drain();
			input.close();
			save.close();

			// now play it back from the recorded file
			FileInputStream fis = new FileInputStream("audio.raw");
			SourceDataLine out = AudioSystem.getSourceDataLine(format);	
			out.open();
			out.start();
			for (int i=0;i<5;i++) {
				fis.read(data);
				out.write(data,0,size);	
			}
			out.drain();
			out.close();
			fis.close();

		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (LineUnavailableException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}
}
