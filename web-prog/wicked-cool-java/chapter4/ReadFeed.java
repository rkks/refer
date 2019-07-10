package com.wickedcooljava.web;

import java.io.IOException;
import de.nava.informa.core.ParseException;
import de.nava.informa.impl.basic.Channel;
import de.nava.informa.impl.basic.ChannelBuilder;
import de.nava.informa.impl.basic.Item;
import de.nava.informa.parsers.FeedParser;

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
 * This example reads an RSS newsfeed and prints its details to the console.
 */
public class ReadFeed {

	public static void main(String[] args) 
	throws ParseException, IOException {
		ChannelBuilder builder = new ChannelBuilder();
		String url = "http://wickedcooljava.com/updates.rss";
		Channel channel = (Channel) FeedParser.parse(builder, url);
		System.out.println("Description: " + channel.getDescription());
		System.out.println("Title: " + channel.getTitle());
		System.out.println("======================================");
		for (Object x : channel.getItems())
		{
		    Item anItem = (Item) x;
		    System.out.print(anItem.getTitle() + " - ");
		    System.out.println(anItem.getDescription());
		}
	}
}
