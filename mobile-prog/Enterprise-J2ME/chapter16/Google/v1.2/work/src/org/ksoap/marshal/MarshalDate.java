package org.ksoap.marshal;

import java.util.Date;
import java.io.*;
import org.ksoap.*;
import org.kobjects.isodate.*;
import org.kobjects.serialization.*;

/** Marshal class for Dates. */


public class MarshalDate implements Marshal {

    public static Class DATE_CLASS = new Date ().getClass ();


    public Object readInstance (SoapParser parser,
				String namespace, String name,
				ElementType expected) throws IOException {

	parser.parser.read (); // start tag
	Object result = IsoDate.stringToDate 
	    (parser.parser.readText (), IsoDate.DATE_TIME);
	parser.parser.read (); // end tag
	return result;
    }
	

    public void writeInstance (SoapWriter writer, 
			       Object obj) throws IOException {

	writer.writer.write (IsoDate.dateToString 
			     ((Date) obj, IsoDate.DATE_TIME));
    }

    
    public void register (ClassMap cm) {
	cm.addMapping 
	    (cm.xsd, "dateTime", 
	     MarshalDate.DATE_CLASS, this);
    }
    
}
