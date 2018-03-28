package org.ksoap.marshal;

import java.io.*;
import org.ksoap.*;
import org.kobjects.serialization.*;


/** This marshal class is able to handle float, double and decimal.
    Since CLDC does not contain support for floating point
    arithmetics, it will work only with J2SE. */


public class MarshalFloat implements Marshal {


    public Object readInstance (SoapParser parser,
				String namespace, String name,
				ElementType expected) throws IOException {

	parser.parser.read (); // start tag
	Object result;
	if (name.equals ("float"))
	    result = new Float (parser.parser.readText ());
	else if (name.equals ("double")) 
	    result = new Double (parser.parser.readText ());
	else if (name.equals ("decimal"))
	    result = new java.math.BigDecimal (parser.parser.readText ());
	else
	    throw new RuntimeException ("float or double expected");
	parser.parser.read (); // end tag
	return result;
    }


    public void writeInstance (SoapWriter writer, 
			       Object instance) throws IOException {
	
	writer.writer.write (instance.toString ());
    }
    

    public void register (ClassMap cm) {
	cm.addMapping (cm.xsd, "float", Float.class, this);
	cm.addMapping (cm.xsd, "double", Double.class, this);
	cm.addMapping (cm.xsd, "decimal", java.math.BigDecimal.class, this);
    }
}

