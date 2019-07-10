
package com.wickedcooljava.graphics;

import java.awt.Color;
import java.awt.geom.Rectangle2D;
import java.util.Random;

import javax.swing.JFrame;
import javax.swing.JScrollPane;

import org._3pq.jgrapht.ext.JGraphModelAdapter;
import org._3pq.jgrapht.graph.ListenableDirectedGraph;
import org.jgraph.JGraph;
import org.jgraph.graph.AttributeMap;
import org.jgraph.graph.CellView;
import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.EdgeView;
import org.jgraph.graph.GraphCell;
import org.jgraph.graph.GraphConstants;
import org.jgraph.graph.GraphLayoutCache;
import org.jgraph.layout.CircleGraphLayout;
import org.jgraph.layout.JGraphLayoutAlgorithm;

import com.wickedcooljava.sci.HumanOrgansGraph;

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
 * An example using the JGraph API to display data from a graph.
 * This uses the Human Organs example graph from Chapter 5.
 */
public class HumanOrgansView {

	JGraph jgraph;
	
	public HumanOrgansView() {
		// use a JGraphT listenable graph
		ListenableDirectedGraph graph = new ListenableDirectedGraph();
		// create the view, then add data to the model
		JGraphModelAdapter adapter = new JGraphModelAdapter(graph, null, null);
		jgraph = new JGraph(adapter);
		JScrollPane scroller = new JScrollPane(jgraph);
		JFrame frame = new JFrame("The Body");
		frame.setSize(600,600);
		frame.add(scroller);
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		// now add the data
		HumanOrgansGraph hog = new HumanOrgansGraph(graph);
		removeEdgeLabels();
		setColors();
		CircleGraphLayout layout = new CircleGraphLayout();
		layout.run(jgraph, jgraph.getRoots(), new Object[]{});
		jgraph.getGraphLayoutCache().reload();
		jgraph.repaint();
	}
	
	/**
	 * Removing edge labels
	 */
	public void removeEdgeLabels() {
		GraphLayoutCache cache = jgraph.getGraphLayoutCache();
		CellView[] cells = cache.getCellViews();
		for (CellView cell : cells) {
			if (cell instanceof EdgeView) {
				EdgeView ev = (EdgeView) cell;
				DefaultEdge eval = (DefaultEdge) ev.getCell();
				eval.setUserObject("");
			}
		}
		cache.reload();
		jgraph.repaint();
	}
	
	/**
	 * Setting vertex colors
	 */
	public void setColors() {
		GraphLayoutCache cache = jgraph.getGraphLayoutCache();
		for (Object item : jgraph.getRoots()) {
			GraphCell cell = (GraphCell) item;
			CellView view = cache.getMapping(cell, true);
			AttributeMap map = view.getAttributes();
			map.applyValue(GraphConstants.BACKGROUND, Color.GREEN);
		}
		cache.reload();
		jgraph.repaint();
	}
	
	/** 
	 * Example of how to randomize vertex locations
	 */
	public void randomizeLocations() {
		GraphLayoutCache cache = jgraph.getGraphLayoutCache();
		Random r = new Random();
		for (Object item : jgraph.getRoots()) {
			GraphCell cell = (GraphCell) item;
			CellView view = cache.getMapping(cell, true);
			Rectangle2D bounds = view.getBounds();
			bounds.setRect(r.nextDouble() * 400, r.nextDouble() * 400, bounds.getWidth(), bounds.getHeight());
		}
		cache.reload();
		jgraph.repaint();
	}
	
	public static void main(String[] args) {
		new HumanOrgansView();
	}
}
