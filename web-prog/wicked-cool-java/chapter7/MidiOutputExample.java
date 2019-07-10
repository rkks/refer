package com.wickedcooljava.media;

import javax.sound.midi.MidiDevice;
import javax.sound.midi.MidiMessage;
import javax.sound.midi.MidiSystem;
import javax.sound.midi.MidiUnavailableException;
import javax.sound.midi.Receiver;

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
 * Example sending output to a MIDI device.
 */
public class MidiOutputExample {

	public static void main(String[] args) {
		try {
			MidiDevice.Info[] devices = MidiSystem.getMidiDeviceInfo();
			for (MidiDevice.Info deviceInfo : devices) {
				System.out.println(deviceInfo.getDescription());
				MidiDevice device = MidiSystem.getMidiDevice(deviceInfo);
				device.open();
				Receiver receiver = device.getReceiver();
				// replace this with a real MidiMessage
				MidiMessage msg;
				// use a real timestamp value from a MidiEvent
				long timeStamp;
				receiver.send(msg, timeStamp);
				receiver.close();
				device.close();

			}
		} catch (MidiUnavailableException e) {
			e.printStackTrace();
		}
	}
}
