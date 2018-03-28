

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
 * $Id: Movie.java,v 1.2 2003/02/26 02:37:17 ro89390 Exp $
 */
package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class Movie {
    private String primaryKey;
    private String title;
    private String summary;
    private String rating;
    private boolean alreadySeen = false;
    transient private byte[] poster = null;

    public Movie(String primaryKey, String title, String summary, 
                 String rating) {
        this.primaryKey = primaryKey;
        this.title = title;
        this.summary = summary;
        this.rating = rating;

        return;
    }

    private Movie() {}

    public String getPrimaryKey() {
        return primaryKey;
    } 

    public String getTitle() {
        return title;
    } 

    public String getSummary() {
        return summary;
    } 

    public String getRating() {
        return rating;
    } 

    public void setAlreadySeen(boolean alreadySeen) {
        this.alreadySeen = alreadySeen;

        return;
    } 

    public boolean isAlreadySeen() {
        return alreadySeen;
    } 

    public byte[] getPoster() throws ApplicationException {
        if (poster == null) {
            try {
                poster = 
                    ModelObjectLoader.getInstance().getMoviePoster(primaryKey);
            } catch (ModelException me) {
                throw new ApplicationException();
            } 
        } 

        return poster;
    } 

    public boolean equals(Object object) {
        return object != null 
               && (object instanceof Movie 
                   && (object == this 
                       || ((Movie) object).getPrimaryKey().equals(primaryKey)));
    } 

    public void serialize(DataOutputStream dataStream) 
            throws ApplicationException {
        try {
            dataStream.writeUTF(primaryKey);
            dataStream.writeUTF(title);
            dataStream.writeUTF(summary);
            dataStream.writeUTF(rating);
            dataStream.writeBoolean(alreadySeen);

            return;
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public static Movie deserialize(DataInputStream dataStream) 
            throws ApplicationException {
        try {
            Movie movie = new Movie();

            movie.primaryKey = dataStream.readUTF();
            movie.title = dataStream.readUTF();
            movie.summary = dataStream.readUTF();
            movie.rating = dataStream.readUTF();

            try {
                movie.alreadySeen = dataStream.readBoolean();
            } catch (IOException ioe) {
                movie.alreadySeen = false;
            } 

            try {
                return ModelObjectLoader.getInstance().getMovie(movie);
            } catch (ModelException me) {
                throw new ApplicationException();
            } 
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

}

