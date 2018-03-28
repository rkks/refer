

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
 * $Id: MovieRating.java,v 1.6 2003/03/24 22:43:12 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;

public class MovieRating implements Synchronizable {
    public static final int RATING_UNRATED = 0;
    public static final int RATING_ONE_STAR = 1;
    public static final int RATING_TWO_STARS = 2;
    public static final int RATING_THREE_STARS = 3;
    public static final int RATING_FOUR_STARS = 4;
    public static final int RATING_FIVE_STARS = 5;

    /**
     * @link aggregation
     */
    private String primaryKey;
    private Movie movie = null;
    private int starNumber;
    private long lastViewingDate;
    private long lastModificationDate;
    private int modificationStatus;
    private boolean modifiedOnClient;

    public MovieRating(String primaryKey, int starNumber, 
                       long lastViewingDate, long lastModificationDate, 
                       int modificationStatus, boolean modifiedOnClient) {
        this.primaryKey = primaryKey;
        this.starNumber = starNumber;
        this.lastViewingDate = lastViewingDate;
        this.lastModificationDate = lastModificationDate;
        this.modificationStatus = modificationStatus;
        this.modifiedOnClient = modifiedOnClient;

        return;
    }

    public MovieRating(Movie movie, long lastViewingDate) {
        this(movie, RATING_UNRATED, lastViewingDate, 
             System.currentTimeMillis(), Synchronizable.STATUS_INSERTED, 
             true);

        return;
    }

    private MovieRating(Movie movie, int starNumber, long lastViewingDate, 
                        long lastModificationDate, int modificationStatus, 
                        boolean modifiedOnClient) {
        this.movie = movie;
        this.primaryKey = movie.getPrimaryKey();
        this.starNumber = starNumber;
        this.lastViewingDate = lastViewingDate;
        this.lastModificationDate = lastModificationDate;
        this.modificationStatus = modificationStatus;
        this.modifiedOnClient = modifiedOnClient;

        return;
    }

    private MovieRating() {}

    public String getPrimaryKey() {
        return primaryKey;
    } 

    public Movie getMovie() throws ApplicationException {
        if (movie == null) {
            try {
                movie = ModelObjectLoader.getInstance().getMovie(primaryKey);
            } catch (ModelException me) {
                throw new ApplicationException();
            } 
        } 

        return movie;
    } 

    public int getStarNumber() {
        return starNumber;
    } 

    public void setStarNumber(int starNumber) {    // Should assert the validity of starNumber
        this.starNumber = starNumber;
        this.lastModificationDate = System.currentTimeMillis();
        ;
        this.modificationStatus = Synchronizable.STATUS_MODIFIED;

        return;
    } 

    public long getLastViewingDate() {
        return lastViewingDate;
    } 

    public void setLastViewingDate(long viewingDate) {
        this.lastViewingDate = lastViewingDate;
        this.lastModificationDate = System.currentTimeMillis();
        ;
        this.modificationStatus = Synchronizable.STATUS_MODIFIED;

        return;
    } 

    public long getLastModificationDate() {
        return lastModificationDate;
    } 

    public int getModificationStatus() {
        return modificationStatus;
    } 

    public void setModificationStatus(int modificationStatus) {    // Should
        // assert the validity of modificationStatus
        this.lastModificationDate = System.currentTimeMillis();
        ;
        this.modificationStatus = modificationStatus;
        modifiedOnClient = true;

        return;
    } 

    public boolean isModifiedOnClient() {
        return modifiedOnClient;
    } 

    public void serialize(DataOutputStream dataStream) 
            throws ApplicationException {
        try {
            dataStream.writeByte(modificationStatus);
            dataStream.writeLong(lastModificationDate);
            dataStream.writeUTF(primaryKey);
            dataStream.writeByte(starNumber);
            dataStream.writeLong(lastViewingDate);
            dataStream.writeBoolean(modifiedOnClient);

            return;
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public static MovieRating deserialize(DataInputStream dataStream) 
            throws ApplicationException {
        try {
            MovieRating movieRating = new MovieRating();

            movieRating.modificationStatus = dataStream.readByte();
            movieRating.lastModificationDate = dataStream.readLong();

            try {
                movieRating.primaryKey = dataStream.readUTF();

                if (ModelObjectLoader.getInstance() 
                        != null) {    // Because server-side model does not provide a ModelObjectLoader
                    movieRating.movie = 
                        ModelObjectLoader.getInstance().getMovie(movieRating.primaryKey);
                } 
            } catch (ModelException me) {
                throw new ApplicationException();
            } 

            movieRating.starNumber = dataStream.readByte();
            movieRating.lastViewingDate = dataStream.readLong();
            movieRating.modifiedOnClient = dataStream.readBoolean();

            return movieRating;
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public boolean equals(Object object) {
        return object != null 
               && (object instanceof MovieRating 
                   && (object == this 
                       || ((MovieRating) object).getPrimaryKey().equals(primaryKey)));
    } 

    // For debugging only (downsizing candidate)
    public String toString() {
        return modificationStatus + ":" + lastModificationDate + ":" 
               + getPrimaryKey() + ":" + starNumber + ":" + lastViewingDate;
    } 

}

