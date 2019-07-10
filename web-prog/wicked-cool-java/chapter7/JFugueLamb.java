package com.wickedcooljava.media;

import org.jfugue.Pattern;
import org.jfugue.Player;

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
 * Example playing Mary Had a Little Lamb, and a chord progression, 
 * using JFugue.
 */
public class JFugueLamb {

	public static void main(String[] args) {
		Pattern lamb = new Pattern();
		lamb.add("eq dq cq dq eq eq eh");
		lamb.add("dq dq dh eq gq gh");
		lamb.add("eq dq cq dq eq eq eq eq");
		lamb.add("dq dq eq dq cw");
		Player jukebox = new Player();
		jukebox.play(lamb);
		
		Pattern progression = new Pattern();
		progression.add("c5majw g5majw a5minw f5majw g5majw c5majw");
		
		jukebox.save(lamb, "/music/lamb.mid");
	}

}
