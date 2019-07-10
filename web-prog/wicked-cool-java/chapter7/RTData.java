package com.wickedcooljava.media;

import javolution.realtime.RealtimeObject;

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
 * A sample real-time data object using Javolution API
 */
public class RTData extends RealtimeObject {
	private long acquireDate;
	private double telemetry;
	private double uncertaincy;
	
	private static final Factory<RTData> FACTORY = new Factory<RTData>() {
		protected RTData create() {
			return new RTData();
		}
	};
	
	private RTData() {}
	
	public static RTData valueOf(long acqDate, double tele, double uncert) {
		RTData data = FACTORY.object();
		data.acquireDate = acqDate;
		data.telemetry = tele;
		data.uncertaincy = uncert;
		return data;
	}
	
	public long getAcquireDate() {
		return acquireDate;
	}
	
	public double getTelemetry() {
		return telemetry;
	}
	
	public double getUncertaincy() {
		return uncertaincy;
	}
}
