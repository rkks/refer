package com.wickedcooljava.sci;

import org._3pq.jgrapht.Edge;
import org._3pq.jgrapht.Graph;
import org._3pq.jgrapht.graph.ListenableDirectedGraph;

import com.wickedcooljava.sci.component.Component;
import com.wickedcooljava.sci.component.InputPort;
import com.wickedcooljava.sci.component.OutputPort;

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
 * This is a simple version of a MetaComponent. It uses a graph 
 * to maintain the connections between child components.
 */
public class MetaComponentSimple<T> {
	// the graph that maintains child components
	private ListenableDirectedGraph graph;

	public MetaComponentSimple() {
		graph = new ListenableDirectedGraph();
	}

	/**
	 * Connect an output port to an input port.
	 */
	public void connect(OutputPort<T> out, InputPort<T> in) {
		Component<T> source = out.getParent();
		Component<T> target = in.getParent();
		// add parent components to graph
		if (!graph.containsVertex(source)) {
			graph.addVertex(source);
		}
		if (!graph.containsVertex(target)) {
			graph.addVertex(target);
		}
		// add ports to graph
		if (!graph.containsVertex(in)) {
			graph.addVertex(in);
		}
		if (!graph.containsVertex(out)) {
			graph.addVertex(out);
		}
		// add an edge from out parent to output port
		graph.addEdge(source, out);
		// add an edge from output port to input port
		graph.addEdge(out, in);
		// add an edge from input port to target component
		graph.addEdge(in, target);
	}

	/**
	 * Perform the processing by processing each of the subcomponents
	 * and propagating signals from outputs to inputs.
	 */
	public void process() {
		processSubComponents();
		propagateSignals();
	}

	/**
	 * Propagate signals from all outputs to all inputs 
	 * for connected subcomponents.
	 */
	private void propagateSignals() {
		for (Object item : graph.edgeSet()) {
			Edge edge = (Edge) item;
			Object source = edge.getSource();
			Object target = edge.getTarget();
			if (source instanceof OutputPort) {
				OutputPort<T> out = (OutputPort<T>) source;
				InputPort<T> in = (InputPort<T>) target;
				in.setValue(out.getValue());
			}
		}
	}

	/**
	 * Process all subcomponents, by calling the process methods for each.
	 */
	private void processSubComponents() {
		for (Object item : graph.vertexSet()) {
			if (item instanceof Component) {
				((Component<T>) item).process();
			}
		}
	}
	
	/**
	 * Returns the graph used by this MetaComponent
	 */
	public Graph getGraph() {
		return graph;
	}

}