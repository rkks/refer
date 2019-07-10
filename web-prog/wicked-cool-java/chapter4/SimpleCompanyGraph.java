package com.wickedcooljava.web;

import com.hp.hpl.jena.rdf.model.Model;
import com.hp.hpl.jena.rdf.model.ModelFactory;
import com.hp.hpl.jena.rdf.model.Property;
import com.hp.hpl.jena.rdf.model.Resource;
import com.hp.hpl.jena.vocabulary.RDFS;

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
 * This class creates a simple RDF graph of company/employee relations using Jena.
 */
public class SimpleCompanyGraph {

	public static void main(String[] args) {
		Model model = ModelFactory.createDefaultModel();
		model.setNsPrefix("wcj", "http://example.org/wcjava/");
		Resource employee = model.createResource("wcj:employee");
		Resource person = model.createResource("wcj:person");
		Resource employer = model.createResource("wcj:employer");
		Resource organization = model.createResource("wcj:organization");
		Property hires = model.createProperty("wcj:hires");
		model.add(employer, hires, employee);
		model.add(employer, RDFS.subClassOf, organization);
		model.add(employee, RDFS.subClassOf, person);
		model.write(System.out, "N3");
	}
}
