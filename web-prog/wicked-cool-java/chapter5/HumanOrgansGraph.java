package com.wickedcooljava.sci;

import java.util.List;

import org._3pq.jgrapht.Edge;
import org._3pq.jgrapht.Graph;
import org._3pq.jgrapht.graph.ListenableDirectedGraph;

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
 * Demo of JGraphT API. 
 * This version has been modified to work with the JGraph example
 * in Chapter 6.
 */
public class HumanOrgansGraph {
	
	enum Organs {HEART, LUNG, LIVER, STOMACH, BRAIN, SPINAL_CORD};
	enum Systems {CIRCULATORY, DIGESTIVE, NERVOUS, RESPIRATORY};
	
	ListenableDirectedGraph graph = null;
	
	/**
	 * Creates an instance using the provided graph.
	 * @param g The graph to use, or null to create a new one.
	 */
	public HumanOrgansGraph(ListenableDirectedGraph g)
	{
		if (g == null) {
			g = new ListenableDirectedGraph();
		}	
		graph = g;
		// add vertices to the graph
		g.addVertex(Organs.HEART);
		g.addVertex(Organs.LUNG);
		g.addVertex(Organs.BRAIN);
		g.addVertex(Organs.STOMACH);
		g.addVertex(Organs.LIVER);
		g.addVertex(Organs.SPINAL_CORD);
		g.addVertex(Systems.CIRCULATORY);
		g.addVertex(Systems.NERVOUS);
		g.addVertex(Systems.DIGESTIVE);
		g.addVertex(Systems.RESPIRATORY);
		// link the vertices by edges
		g.addEdge(Organs.HEART, Systems.CIRCULATORY);
		g.addEdge(Organs.LUNG, Systems.RESPIRATORY);
		g.addEdge(Organs.BRAIN, Systems.NERVOUS);
		g.addEdge(Organs.SPINAL_CORD, Systems.NERVOUS);
		g.addEdge(Organs.STOMACH, Systems.DIGESTIVE);
		g.addEdge(Organs.LIVER, Systems.DIGESTIVE);
		
		// traverse the edges connected to DIGESTIVE vertex
		List digestiveLinks = g.edgesOf(Systems.DIGESTIVE);
		System.out.println(
				digestiveLinks.size() + " digestive organs in the graph");
		for (Object item : digestiveLinks) {
		   Edge anEdge = (Edge) item;
		   Object opposite = anEdge.oppositeVertex(Systems.DIGESTIVE);
		   System.out.println(opposite);
		}
	}

	/**
	 * Provide access to the graph, for use by chapter 6 code.
	 */
	public Graph getGraph()
	{
		return graph;
	}
	
	public static void main(String[] args) {
		new HumanOrgansGraph(null);
	}
}
