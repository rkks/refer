package com.wickedcooljava.media;

import jm.audio.AudioObject;
import jm.audio.Instrument;
import jm.audio.synth.Add;
import jm.audio.synth.Filter;
import jm.audio.synth.Oscillator;

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
 * Using JMusic to synthesize a filtered square wave and sawtooth wave.
 */
public final class SquareFilteredSaw extends Instrument {
	public void createChain() {
		Oscillator saw = new Oscillator(this, Oscillator.SAWTOOTH_WAVE, 44100,
				2);
		Filter filter = new Filter(saw, 500.0, Filter.LOW_PASS);
		Oscillator square = new Oscillator(this, Oscillator.SQUARE_WAVE, 44100,
				2);
		Filter filter2 = new Filter(square, 4000, Filter.HIGH_PASS);
		Add adder = new Add(new AudioObject[] { filter, filter2});
	}
	
}
