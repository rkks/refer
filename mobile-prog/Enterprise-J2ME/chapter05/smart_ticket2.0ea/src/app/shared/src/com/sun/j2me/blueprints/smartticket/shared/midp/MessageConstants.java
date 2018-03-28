

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
 * $Id: MessageConstants.java,v 1.3 2003/03/15 00:49:25 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.shared.midp;

public final class MessageConstants {
    public static final byte OPERATION_LOGIN_USER = 0;
    public static final byte OPERATION_CREATE_ACCOUNT = 1;
    public static final byte OPERATION_UPDATE_ACCOUNT = 2;
    public static final byte OPERATION_GET_THEATERS = 3;
    public static final byte OPERATION_GET_THEATER_SCHEDULE = 4;
    public static final byte OPERATION_GET_MOVIE = 5;
    public static final byte OPERATION_GET_MOVIE_POSTER = 6;
    public static final byte OPERATION_GET_MOVIE_SHOWTIMES = 7;
    public static final byte OPERATION_GET_SEATING_PLAN = 8;
    public static final byte OPERATION_RESERVE_SEATS = 9;
    public static final byte OPERATION_PURCHASE_TICKETS = 10;
    public static final byte OPERATION_CANCEL_SEAT_RESERVATION = 11;
    public static final byte OPERATION_GET_LOCALES = 12;
    public static final byte OPERATION_GET_RESOURCE_BUNDLE = 13;
    public static final byte OPERATION_INITIATE_SYNCHRONIZATION = 14;
    public static final byte OPERATION_SYNCHRONIZE_MOVIE_RATINGS = 15;
    public static final byte OPERATION_COMMIT_MOVIE_RATINGS = 16;
    public static final byte ERROR_NONE = 0;
    public static final byte ERROR_UNKNOWN_OPERATION = 1;
    public static final byte ERROR_SERVER_ERROR = 2;
    public static final byte ERROR_MODEL_EXCEPTION = 3;
    public static final byte ERROR_REQUEST_FORMAT = 4;

    private MessageConstants() {}

}

