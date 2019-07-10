package com.wickedcooljava.misc;

import java.io.IOException;
import java.util.Arrays;

import org.apache.commons.httpclient.Cookie;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.methods.GetMethod;

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
 * Using Commons HttpClient to read data from a web site using a GET request.
 */
public class GetExample {

	public static void main(String[] args) {
		HttpClient client = new HttpClient();
		// an example only... replace this with your site
		GetMethod get = new GetMethod("http://wickedcooljava.com");
		try {
			// get the response code, hopefully "successful" (SC_OK, or 200)
			int response = client.executeMethod(get);
			if (response == HttpStatus.SC_OK) {
				String result = get.getResponseBodyAsString();
				// this is the document returned by the response
				System.out.println(result);
				Cookie[] cookies = client.getState().getCookies();
				System.out.println(Arrays.toString(cookies));
			} else {
				System.err.println("Response code: " + get.getStatusLine());
			}
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			get.releaseConnection();
		}

	}

}
