package org.ksoap;

import java.io.*;
import org.kobjects.serialization.*;

/** Abstract class for custom (de)serialization. */


public interface Marshal {


    /** This methods reads an instance from the given parser. For
        implementation, please note that the start and and tag must be
        consumed. This is not symmetric to writeInstance, but
        otherwise it would not be possible to access the attributes of
        the start tag here. The underlying XML parser is accessible
        using parser.parser. */

    public Object readInstance (SoapParser parser, 
				String namespace, String name,
				ElementType expected) throws IOException;


    /** Write the instance to the given SoapWriter. In contrast to
	readInstance, it is not neccessary to care about the
	sorrounding start and end tags. Additional attributes must be
	writen before anything else is written. The underlying xml
	writer is accessible using writer.writer. */

    public void writeInstance (SoapWriter writer, 
			       Object instance) throws IOException;


    /** Register this Marshal with the given classMap */

    public void register (ClassMap cm);
}





