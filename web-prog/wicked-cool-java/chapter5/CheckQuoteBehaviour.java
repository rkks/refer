
package com.wickedcooljava.sci;

import jade.core.AID;
import jade.core.Agent;
import jade.core.behaviours.TickerBehaviour;
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
 * This class is a test implementation of a JADE behaviour.
 */
public class CheckQuoteBehaviour extends TickerBehaviour {
	Agent myAgent;
	
	public CheckQuoteBehaviour(Agent a) {
		super(a, 300 * 1000); // repeat interval in milliseconds
		myAgent = a;
	}

	protected void onTick() {
		// get the current price from somewhere
		if (getCurPrice() < 5.0) {
			// send a message to the buyer
			AID buyingAgent = new jade.core.AID("buyer",
					AID.ISLOCALNAME);
			// we are requesting that another agent do something
			ACLMessage msg = new ACLMessage(ACLMessage.REQUEST);
			// send a String message
			msg.setContent("buy 100 WHATEVER");
			msg.addReceiver(buyingAgent);
			// have the agent (who owns this behaviour) send the message
			myAgent.send(msg);
		}
	}

	public double getCurPrice() {
		// in a real-world agent, this value would be obtained
		// from somewhere
		return 12.00;
	}
}