/*
 * Copyright 2001 by Sun Microsystems, Inc.,
 * 901 San Antonio Road, Palo Alto, California, 94303, U.S.A.
 * All rights reserved.
 */
package ant;

import java.io.*;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Task;

/**
 * Simple task set a property to the size of a file.
 * It has two attributes, the file name and 
 * the property name. The property set to the empty string
 * if the file does not exist.
 */

public class FileSizeTask extends Task {
    File file;
    String property;

    // The method executing the task
    public void execute() throws BuildException {
	if (property != null && property.length() > 0) {
	    if (file.exists()) {
		int size = (int)file.length();
		project.setProperty(property, Integer.toString(size));
	    } else {
		project.setProperty(property, "");
	    }
	}
    }

    // The setter for the "message" attribute
    public void setFile(File file) {
	this.file = file;
    }

    // The setter for the "property" attribute
    public void setProperty(String prop) {
	property = prop;
    }

}
