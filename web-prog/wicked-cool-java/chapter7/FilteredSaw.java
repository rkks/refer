package com.wickedcooljava.media;

import jm.audio.Instrument;
import jm.audio.synth.Oscillator;
import jm.audio.synth.Filter;
import jm.music.data.Note;
import jm.music.data.Part;
import jm.music.data.Phrase;
import jm.music.data.Score;
import jm.util.Play;

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
 * Example using JMusic to create a filtered sawtooth wave.
 */
public final class FilteredSaw extends Instrument {
	public void createChain() {
		Oscillator saw = new Oscillator(this, Oscillator.SAWTOOTH_WAVE, 44100,
				2);
		Filter filter = new Filter(saw, 500.0, Filter.LOW_PASS);
	}

	public static void main(String[] args) {
		FilteredSaw inst = new FilteredSaw();
		Score score = new Score();
		Part part = new Part();
		part.setTempo(180);
		Phrase phrase = new Phrase();
		phrase.addNote(new Note(C4, QUARTER_NOTE));
		phrase.addNote(new Note(G4, QUARTER_NOTE));
		phrase.addNote(new Note(E4, QUARTER_NOTE));
		phrase.addNote(new Note(F4, QUARTER_NOTE));
		phrase.addNote(new Note(G4, WHOLE_NOTE));
		part.addPhrase(phrase);
		score.add(part);
		Play.audio(score, inst);
	}
}