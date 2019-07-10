package com.wickedcooljava.xml;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.Reader;

import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.index.IndexWriter;

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
 * This class demonstrates indexing content using Lucene.
 */
public class SampleIndexer {

	public static void main(String[] args) throws Exception {
		StandardAnalyzer analyzer = new StandardAnalyzer();
		IndexWriter writer = new IndexWriter("index", analyzer,	true);
		Document doc = new Document();
		FileInputStream is = new FileInputStream("c:/projects/wcj3/sample.txt");
		Reader reader = new BufferedReader(new InputStreamReader(is));
		doc.add(Field.Text("contents", reader));
		doc.add(Field.Text("name", "sample.txt"));
		writer.addDocument(doc);
		Document doc2 = new Document();
		doc2.add(Field.Text("contents", "This is some more data to index"));
		doc2.add(Field.Text("name", "some data"));
		writer.addDocument(doc2);
		writer.optimize();
		writer.close();

	}
}
