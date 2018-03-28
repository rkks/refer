

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
 * $Id: Theater.java,v 1.2 2003/02/26 02:37:17 ro89390 Exp $
 */
package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class Theater {
    private String primaryKey;
    private String name;
    private String address;
    private String zipCode;

    /**
     * @link aggregationByValue
     */
    transient private TheaterSchedule theaterSchedule = null;

    public Theater(String primaryKey, String name, String address, 
                   String zipCode) {
        this.primaryKey = primaryKey;
        this.name = name;
        this.address = address;
        this.zipCode = zipCode;

        return;
    }

    private Theater() {}

    public String getPrimaryKey() {
        return primaryKey;
    } 

    public String getName() {
        return name;
    } 

    public String getAddress() {
        return address;
    } 

    public String getZipCode() {
        return zipCode;
    } 

    public TheaterSchedule getTheaterSchedule() throws ApplicationException {
        if (theaterSchedule == null 
                || theaterSchedule.getExpirationDate() 
                   < System.currentTimeMillis()) {    // ???
            try {
                theaterSchedule = 
                    ModelObjectLoader.getInstance().getTheaterSchedule(this);
            } catch (ModelException me) {
                throw new ApplicationException();
            } 
        } 

        return theaterSchedule;
    } 

    public boolean equals(Object object) {
        return object != null 
               && (object instanceof Theater 
                   && (object == this 
                       || ((Theater) object).getPrimaryKey().equals(primaryKey)));
    } 

    public void serialize(DataOutputStream dataStream) 
            throws ApplicationException {
        try {
            dataStream.writeUTF(zipCode);
            dataStream.writeUTF(primaryKey);
            dataStream.writeUTF(name);
            dataStream.writeUTF(address);

            return;
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public static Theater deserialize(DataInputStream dataStream) 
            throws ApplicationException {
        try {
            Theater theater = new Theater();

            theater.zipCode = dataStream.readUTF();
            theater.primaryKey = dataStream.readUTF();
            theater.name = dataStream.readUTF();
            theater.address = dataStream.readUTF();

            return theater;
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    // Optimization to avoid deserializing and instantiating a Theater when matching RMS records
    public static boolean matches(byte[] data, 
                                  String zipCode) throws IOException {
        ByteArrayInputStream byteStream = new ByteArrayInputStream(data);
        DataInputStream dataStream = new DataInputStream(byteStream);

        return zipCode.equals(dataStream.readUTF());
    } 

}

