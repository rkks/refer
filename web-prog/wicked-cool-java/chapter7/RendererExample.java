package com.wickedcooljava.media;

import javax.sound.midi.MidiEvent;
import javax.sound.midi.Sequence;
import javax.sound.midi.Track;

import org.jfugue.Pattern;
import org.jfugue.Player;
import org.jfugue.Renderer;

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
 * Example using JFugue to create MIDI events.
 */
public class RendererExample {

	public static void main(String[] args) {
		Player jukebox = new Player();
		Pattern song = new Pattern("c5q d5q e5q f5q g5q a5q b5q c6w");
		jukebox.play(song);
		Renderer renderer = new Renderer();
		Sequence sequence = renderer.render(song);
		Track[] tracks = sequence.getTracks();
		// sequence from the previous example
		for (Track aTrack : tracks) {
			for (int i = 0; i < aTrack.size(); i++) {
				MidiEvent event = aTrack.get(i);
				// now you could send the event to a device
			}
		}
	}

}
