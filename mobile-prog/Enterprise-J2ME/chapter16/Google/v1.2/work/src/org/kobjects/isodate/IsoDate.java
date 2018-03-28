// kObjects
//
// Copyright (C) 2001 Stefan Haustein, Oberhausen (Rhld.), Germany
//
// Contributors: Sean McDaniel
//
// License: LGPL
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2.1 of
// the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA




package org.kobjects.isodate;

import java.util.*;

public class IsoDate {

    public static final int DATE = 1;
    public static final int TIME = 2;
    public static final int DATE_TIME = 3;


    static void dd (StringBuffer buf, int i) {
	buf.append ((char) (((int) '0') + i / 10));
	buf.append ((char) (((int) '0') + i % 10));
    }


    public static String dateToString (Date date, int type) {

	Calendar c = Calendar.getInstance ();
	c.setTimeZone (TimeZone.getTimeZone ("GMT"));
	c.setTime (date);


	StringBuffer buf = new StringBuffer ();

	if ((type & DATE) != 0) {
	    int year = c.get (Calendar.YEAR);
	    dd (buf, year / 100);
	    dd (buf, year % 100);
	    buf.append ('-');
	    dd (buf, c.get (Calendar.MONTH) - Calendar.JANUARY + 1);
	    buf.append ('-');
	    dd (buf, c.get (Calendar.DAY_OF_MONTH));

	    if (type == DATE_TIME)
		buf.append ("T");
	}

	if ((type & TIME) != 0) {
	    dd (buf, c.get (Calendar.HOUR_OF_DAY));
	    buf.append (':');
	    dd (buf, c.get (Calendar.MINUTE));
	    buf.append (':');
	    dd (buf, c.get (Calendar.SECOND));
	    buf.append ('.');
	    int ms = c.get (Calendar.MILLISECOND);
	    buf.append ((char) (((int) '0') + (ms / 100)));
	    dd (buf, ms % 100);
	    buf.append ('Z');
	}

	return buf.toString ();
    }



    public static Date stringToDate (String text, int type) {

	Calendar c = Calendar.getInstance ();

	if (type != DATE_TIME)
	    c.setTime (new Date (0));

	if ((type & DATE) != 0) {
	    c.set (Calendar.YEAR, Integer.parseInt
		   (text.substring (0, 4)));
	    c.set (Calendar.MONTH, Integer.parseInt
		   (text.substring (5, 7)) - 1 + Calendar.JANUARY);
	    c.set (Calendar.DAY_OF_MONTH, Integer.parseInt
		   (text.substring (8, 10)));

	    if (type == DATE_TIME)
		text = text.substring (11);
	}

	if ((type & TIME) == 0)
	    return c.getTime ();

	c.set (Calendar.HOUR_OF_DAY, Integer.parseInt
	       (text.substring (0, 2)));              // -11
	c.set (Calendar.MINUTE, Integer.parseInt
	       (text.substring (3, 5)));
	c.set (Calendar.SECOND, Integer.parseInt
	       (text.substring (6, 8)));

	int pos = 8;
	if (pos < text.length() && text.charAt (pos) == '.') {
	    int ms = 0;
	    int f = 100;
	    while (true) {
		char d = text.charAt (++pos);
		if (d < '0' || d > '9') break;
		ms += (d-'0') * f;
		f /= 10;
	    }
	    c.set (Calendar.MILLISECOND, ms);
	}
	else
	    c.set (Calendar.MILLISECOND, 0);

	if (pos < text.length ()) {

	    if (text.charAt (pos) == '+' || text.charAt (pos) == '-')

		c.setTimeZone (TimeZone.getTimeZone
			       ("GMT"+text.substring (pos)));

	    /*	    return new Date
		    (c.getTime ().getTime ()
		    + (Integer.parseInt
		    (text.substring (pos+1, pos+3)) * 60
		    + Integer.parseInt
		    (text.substring (pos+4, pos+6)))
		    * (text.charAt (pos) == '-' ? -60000 : 60000)); */

	    else if (text.charAt (pos) == 'Z')
		c.setTimeZone (TimeZone.getTimeZone ("GMT"));
	    else
		throw new RuntimeException ("illegal time format!");
	}

	return c.getTime ();
    }
}

