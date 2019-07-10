package com.wickedcooljava.sci;

import jade.lang.acl.ACLMessage;

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
 * Example code for reading and responding to a JADE message.
 */
public class PurchasingAgent extends jade.core.Agent {
	protected void setup() {
		addBehaviour(new jade.core.behaviours.CyclicBehaviour(this) {
			public void action() {
				ACLMessage msg = receive();
				if (msg != null) {
					String data = msg.getContent();
					// call some method to do the agent's work
					String result = tryToBuy(data);
					// there is a special shortcut for responding
					ACLMessage reply = msg.createReply();
					reply.setPerformative(ACLMessage.INFORM);
					reply.setContent(result);
					send(reply);
				}
				block();
			}
		});
	}
	
	/**
	 * Stub for some real-world method that attempts to buy stock 
	 */
	public String tryToBuy(String data) {
		return "tried to buy";
	}
}
