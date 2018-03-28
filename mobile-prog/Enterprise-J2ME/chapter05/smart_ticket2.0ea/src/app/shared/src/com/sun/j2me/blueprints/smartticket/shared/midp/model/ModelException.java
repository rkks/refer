

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
 * $Id: ModelException.java,v 1.3 2003/03/19 07:56:52 ro89390 Exp $
 */
package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class ModelException extends Exception {
    public static final byte CAUSE_GENERAL = 0;
    public static final byte CAUSE_UNAVAILABLE_SEATS = 1;
    public static final byte CAUSE_USER_ALREADY_EXISTS = 2;
    public static final byte CAUSE_USER_NOT_FOUND = 3;
    public static final byte CAUSE_INCORRECT_PASSWORD = 4;
    public static final byte CAUSE_USER_NOT_LOGGED_IN = 5;
    public static final byte CAUSE_THEATERS_NOT_FOUND = 6;
    public static final byte CAUSE_THEATER_SCHEDULE_NOT_FOUND = 7;
    public static final byte CAUSE_SHOW_TIMES_NOT_FOUND = 8;
    public static final byte CAUSE_SHOW_NOT_FOUND = 9;
    public static final byte CAUSE_SEATING_PLAN_NOT_FOUND = 10;
    public static final byte CAUSE_RESERVATION_NOT_FOUND = 11;
    public static final byte CAUSE_MOVIE_NOT_FOUND = 12;
    public static final byte CAUSE_MOVIE_POSTER_NOT_FOUND = 12;
    private int causeCode = CAUSE_GENERAL;

    public ModelException(int causeCode) {
        this.causeCode = causeCode;

        return;
    }

    public int getCauseCode() {
        return causeCode;
    } 

    public void serialize(DataOutputStream dataStream) 
            throws ApplicationException {
        try {
            dataStream.writeInt(causeCode);

            return;
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public static ModelException deserialize(DataInputStream dataStream) 
            throws ApplicationException {
        try {
            return new ModelException(dataStream.readInt());
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

}

