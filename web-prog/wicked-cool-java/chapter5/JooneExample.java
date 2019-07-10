package com.wickedcooljava.sci;

import org.joone.engine.Layer;
import org.joone.engine.Monitor;
import org.joone.io.FileOutputSynapse;
import org.joone.net.NeuralNet;
import org.joone.net.NeuralNetLoader;

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
 * Code to demonstrate how to run neural nets created in the 
 * Joone editor (via the File | Export NeuralNet menu item).
 */
public class JooneExample {

	public static void main(String[] args) {
		NeuralNetLoader netLoader = new NeuralNetLoader(
				"/projects/nn/mynetwork.snet");
		NeuralNet myNet = netLoader.getNeuralNet();
		// get the output layer of the network
		Layer output = myNet.getOutputLayer();
		// add output synapse (connection) to the output layer
		FileOutputSynapse myOutput = new FileOutputSynapse();
		// set the output file as mynetwork.out
		myOutput.setFileName("/projects/nn/mynetwork.out");
		output.addOutputSynapse(myOutput);
		Monitor monitor = myNet.getMonitor();
		// execute for one cycle
		monitor.setTotCicles(1);
		// not in learning mode
		monitor.setLearning(false);
		// start the neural net's layers
		myNet.start();
		// start the monitor
		monitor.Go();
	}

}
