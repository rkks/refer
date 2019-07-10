package com.wickedcooljava.media;

import jm.audio.AOException;
import jm.audio.AudioObject;
import jm.audio.Instrument;
import jm.audio.synth.Add;
import jm.audio.synth.Envelope;
import jm.audio.synth.Noise;
import jm.audio.synth.Oscillator;
import jm.audio.synth.StereoPan;
import jm.audio.synth.Volume;

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
 * Using JMusic to synthesize a sine wave with noise.
 */
public class NoisySine extends Instrument {

	public void createChain() throws AOException {
		// this oscillator defaults to the fundamental frequency
		Oscillator sine = new Oscillator(this, Oscillator.SINE_WAVE);
		Envelope sineEnv1 = new Envelope(sine, new double[] { 0.3, 0.5, 0.7,
				0.9, 0.7, 0.5, 0.3 });
		// this oscillator is at the 5th harmonic
		Oscillator sineHigh = new Oscillator(this, Oscillator.SINE_WAVE);
		sineHigh.setFrqRatio(5);
		Envelope sineEnv2 = new Envelope(sineHigh, new double[] { 0.1, 0.2,
				0.3, 1.0, 0.3, 0.2, 0.1 });
		// create a white noise component
		Noise noise = new Noise(this);
		Envelope env = new Envelope(noise, new double[] { 0.01, 0.05, 0.1,
				0.05, 0.01 });
		// add the three sound components together
		Add adder = new Add(new AudioObject[] { sineEnv1, sineEnv2, env });
		Volume vol = new Volume(adder);
		StereoPan pan = new StereoPan(vol);
	}
}
