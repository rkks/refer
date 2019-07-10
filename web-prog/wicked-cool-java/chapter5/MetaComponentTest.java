
package com.wickedcooljava.sci;

import cern.colt.bitvector.BitMatrix;

import com.wickedcooljava.sci.component.BaseComponent;
import com.wickedcooljava.sci.component.Component;
import com.wickedcooljava.sci.component.InputPort;
import com.wickedcooljava.sci.component.OutputPort;
import com.wickedcooljava.sci.component.TruthTableComponentEngine;

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
 * Code to test the SimpleMetaComponent.
 */
public class MetaComponentTest {

	/**
	 * Create an AND gate for the given number of input bits.
	 * @return a component that performs the AND function
	 */
	public static Component<Boolean> createAndGateComponent(int inputs) {
		SimpleTruthTable and = new SimpleTruthTable(inputs, 1);
		int allOnes = (1 << inputs) - 1;
		and.store(allOnes, 1);
		TruthTableComponentEngine processor = new TruthTableComponentEngine(and);
		return new BaseComponent<Boolean>(inputs, 1, processor);
	}

	/**
	 * Create an OR gate for the given number of input bits.
	 * @return a component that performs the OR function
	 */
	public static Component<Boolean> createOrGateComponent(int inputs) {
		SimpleTruthTable or = new SimpleTruthTable(inputs, 1);
		// the number of rows in the OR gate's bit matrix
		// (number of input bit combinations)
		int rows = 1 << inputs;
		// make an OR by setting anything other than all inputs as zero 
		// to output a one/true
		BitMatrix orMat = new BitMatrix(1, rows);
		orMat.toBitVector().replaceFromToWith(1, rows - 1, true);
		or.setTruthMatrix(orMat);
		// create a component based on the truth table
		TruthTableComponentEngine processor = new TruthTableComponentEngine(or);
		return new BaseComponent<Boolean>(inputs, 1, processor);
	}

	
	/**
	 * Example code for using the metacomponent.
	 * 
	 */
	public static void main(String[] args) {
		MetaComponentSimple<Boolean> manager = new MetaComponentSimple<Boolean>();
		// Assume that we have created an AND gate Component,
		// using techniques discussed earlier. This one has 2 inputs.
		Component<Boolean> and = createAndGateComponent(2);
		OutputPort<Boolean> y = and.getOutputPort(0);
		// We'll use a couple of OR gates, each with 2 inputs
		Component<Boolean> or1 = createOrGateComponent(2);
		InputPort<Boolean> a = or1.getInputPort(0);
		InputPort<Boolean> b = or1.getInputPort(1);
		Component<Boolean> or2 = createOrGateComponent(2);
		InputPort<Boolean> c = or2.getInputPort(0);
		InputPort<Boolean> d = or2.getInputPort(1);
		manager.connect(or1.getOutputPort(0), and.getInputPort(0));
		manager.connect(or2.getOutputPort(0), and.getInputPort(1));
		// set the input values
		a.setValue(true);
		b.setValue(false);
		c.setValue(false);
		d.setValue(false);
		manager.process();
		// we need to do this twice because there are two stages
		manager.process();
		System.out.println(y);     // this prints false	
	}

}
