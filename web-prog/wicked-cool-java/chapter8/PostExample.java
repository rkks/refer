package com.wickedcooljava.misc;

import java.io.IOException;

import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpException;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.methods.PostMethod;

/*
 * Example code from Wicked Cool Java (No Starch Press) Copyright (C) 2005 Brian
 * D. Eubanks
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 * 
 * Note: The LGPL licence can be found online at http://www.gnu.org
 * 
 */
/**
 * Using Commons HttpClient to submit data to a web site,
 * and process the response, using a POST request.
 */
public class PostExample {

	public static void main(String[] args) {
		// do a search of DMOZ, using a POST
		HttpClient client = new HttpClient();
		String results = "";
		PostMethod method = new PostMethod(
				"http://search.dmoz.org/cgi-bin/search");
		method.addParameter("search", "coffee");
		try {
			int respcode = client.executeMethod(method);
			if (respcode == HttpStatus.SC_OK) {
				results = method.getResponseBodyAsString();
			} else {
				System.err.println("Failure: " + method.getStatusLine());
			}
		} catch (HttpException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			method.releaseConnection();
		}
		System.out.println("--- Results obtained ---");
		System.out.println(results);
	}

}
