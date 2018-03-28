

/*
 * Copyright 2001, 2002, 2003 Sun Microsystems, Inc. All Rights Reserved.
 * Except for any files in PNG format (which are marked with the filename
 * extension ".png"), GIF format (which are marked with the filename
 * extension ".gif"), or JPEG format (which are marked with the filename
 * extension ".jpg"), redistribution and use in source and binary forms,
 * with or without modification, are permitted provided that the
 * following conditions are met:
 * - Redistribution of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * - Redistribution in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * You may compile, use, perform and display the following files with
 * original Java Smart Ticket Sample Application code obtained from Sun
 * Microsystems, Inc. only:
 * - files in PNG format and having the ".png" extension
 * - files in GIF format and having the ".gif" extension
 * - files in JPEG format and having the ".jpg" extension
 * You may not modify or redistribute .png, .gif, or .jpg files in any
 * form, in whole or in part, by any means without prior written
 * authorization from Sun Microsystems, Inc. and its licensors, if any.
 * Neither the name of Sun Microsystems, Inc., the 'Java Smart Ticket
 * Sample Application', 'Java', 'Java'-based names, or the names of
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN
 * MIDROSYSTEMS, INC. ("SUN") AND ITS LICENSORS SHALL NOT BE LIABLE FOR
 * ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
 * DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR
 * ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR
 * DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE
 * DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY,
 * ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF
 * SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * You acknowledge that this software is not designed, licensed or
 * intended for use in the design, construction, operation or maintenance
 * of any nuclear facility.
 * $Id: IndexedResourceBundle.java,v 1.3 2003/03/15 02:50:15 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.shared.midp;

import java.io.*;
import java.util.*;

/**
 */
public class IndexedResourceBundle {
    private static IndexedResourceBundle instance = null;
    private String locale;
    private String[] resources;

    public IndexedResourceBundle(String locale, String[] resources) {
        this.locale = locale;
        this.resources = resources;

        return;
    }

    public String getString(int resourceId) {
        return (resourceId >= 0 && resourceId < resources.length) 
               ? resources[resourceId] : null;
    } 

    public String getLocale() {
        return locale;
    } 

    public int size() {
        return resources.length;
    } 

    public void serialize(DataOutputStream out) throws ApplicationException {
        try {
            out.writeUTF(locale);
            out.writeInt(resources.length);

            for (int i = 0; i != resources.length; i++) {
                out.writeUTF(resources[i]);
            } 
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public static IndexedResourceBundle deserialize(DataInputStream in) 
            throws ApplicationException {
        try {
            String locale = in.readUTF();
            String[] resources = new String[in.readInt()];

            for (int i = 0; i != resources.length; i++) {
                resources[i] = in.readUTF();
            } 

            return new IndexedResourceBundle(locale, resources);
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public static IndexedResourceBundle getBundleFromPropertyFile(String locale, 
            InputStream in) throws IOException {
        Vector resourcesVector = new Vector();
        ByteArrayOutputStream out = null;

        try {
            out = new ByteArrayOutputStream();

            int c;
            int index = 0;

            while ((c = in.read()) != -1) {
                if (c == '\n' || c == '\r') {
                    String s = out.toString();
                    int i = s.indexOf('=');

                    if (i != -1) {
                        if (s.substring(0, i).equals(String.valueOf(index))) {
                            resourcesVector.addElement(s.substring(i + 1));

                            index++;
                        } else {
                            // FIXME - Throw an exception.
                        } 
                    } 

                    out.reset();
                } else {
                    out.write(c);
                } 
            } 
        } 
        finally {
            if (out != null) {
                out.close();
            } 

            if (in != null) {
                in.close();
            } 
        } 

        String[] resources = new String[resourcesVector.size()];

        resourcesVector.copyInto(resources);

        return new IndexedResourceBundle(locale, resources);
    } 

    public String toString() {
        StringBuffer buffer = new StringBuffer();

        buffer.append("Contents of resource bundle: " + locale + "\n");

        for (int i = 0; i != resources.length; i++) {
            buffer.append(i);
            buffer.append('=');
            buffer.append(resources[i]);
            buffer.append('\n');
        } 

        return buffer.toString();
    } 

}

