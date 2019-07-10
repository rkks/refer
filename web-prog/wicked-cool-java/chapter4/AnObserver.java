package com.wickedcooljava.web;

import de.nava.informa.core.ChannelIF;
import de.nava.informa.core.ItemIF;
import de.nava.informa.utils.poller.PollerObserverIF;

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
 * This class implements a simple RSS poller observer.
 */
public class AnObserver implements PollerObserverIF
{
	public void itemFound(ItemIF item, ChannelIF channel) {
		System.out.println("New item found");
		channel.addItem(item);
	}

	public void pollStarted(ChannelIF channel) {
		System.out.println(
			"Started poll with " + channel.getItems().size() +
			" items in channel");
	}
	
	public void pollFinished(ChannelIF channel) {
		System.out.println(
			"Finished poll with " + channel.getItems().size() + 
			" items in channel");
	}
	
	public void channelChanged(ChannelIF channel) {}
	public void channelErrored(ChannelIF channel, Exception e) {}	
}

