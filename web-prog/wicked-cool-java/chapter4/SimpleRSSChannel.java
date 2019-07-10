package com.wickedcooljava.web;

import java.net.URL;
import de.nava.informa.core.ChannelExporterIF;
import de.nava.informa.core.ChannelIF;
import de.nava.informa.core.ItemIF;
import de.nava.informa.exporters.RSS_1_0_Exporter;
import de.nava.informa.impl.basic.ChannelBuilder;

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
 * This class creates a simple RSS feed.
 */
public class SimpleRSSChannel {

	public static void main(String[] args) throws Exception {
		ChannelBuilder builder = new ChannelBuilder();
		ChannelIF myChannel = builder.createChannel("Latest Bug Fixes");
		// replace the following URL with the location of your RSS document
		URL channelURL = new URL("http://example.org/wcj/bugs.rss");
		myChannel.setLocation(channelURL);
		myChannel.setDescription("The latest news on our bug fixes");

		String title = "Annoying Bug #25443 Now Fixed";
		String desc = "A major bug in OurGreatApplication is fixed. "
				+ "Bug #25443, which has been annoying users ever since 3.0, "
				+ "was due to a rogue null pointer.";
		URL url = new URL("http://example.org/wcj/bugfix25443.html");
		ItemIF anItem = builder.createItem(myChannel, title, desc, url);
		anItem.setCreator("Ecks Amples");

		title = "Bug #12121 not Fixed in OurGreatApplication 7.1";
		desc = "Bug #12121 will not be fixed in OurGreatApplication "
				+ "release 7.1, so that developers can focus on adding "
				+ "the WickedCool feature.";
		url = new URL("http://example.org/wcj/bugfix12121.html");
		anItem = builder.createItem(myChannel, title, desc, url);
		anItem.setCreator("Dee Veloper");

		ChannelExporterIF exporter = new RSS_1_0_Exporter("bugs.rss");
		exporter.write(myChannel);
	}
}
