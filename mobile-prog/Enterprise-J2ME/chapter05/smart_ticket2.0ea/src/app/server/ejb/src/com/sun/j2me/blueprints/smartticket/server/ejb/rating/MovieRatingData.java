

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
 * $Id: MovieRatingData.java,v 1.3 2003/03/15 00:49:25 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.ejb.rating;

import java.io.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncConstants;

public class MovieRatingData {

    /**
     * @link aggregation
     */
    private String movieId;
    private int starNumber;
    private long lastViewingDate;
    private long lastModificationDate;
    private int modificationStatus;
    private boolean modifiedOnClient = true;

    public MovieRatingData(String movieId, int starNumber, 
                           long lastViewingDate, long lastModificationDate, 
                           int modificationStatus) {
        this(movieId, starNumber, lastViewingDate, lastModificationDate, 
             modificationStatus, true);

        return;
    }

    public MovieRatingData(String movieId, int starNumber, 
                           long lastViewingDate, long lastModificationDate) {
        this(movieId, starNumber, lastViewingDate, lastModificationDate, 
             SyncConstants.STATUS_UNCHANGED, false);

        return;
    }

    public MovieRatingData(String movieId, long lastModificationDate) {
        this(movieId, MovieRatingConstants.RATING_UNRATED, 0L, 
             lastModificationDate, SyncConstants.STATUS_DELETED, false);

        return;
    }

    public MovieRatingData(String movieId, int starNumber, 
                           long lastViewingDate, long lastModificationDate, 
                           int modificationStatus, boolean modifiedOnClient) {
        this.movieId = movieId;
        this.starNumber = starNumber;
        this.lastViewingDate = lastViewingDate;
        this.lastModificationDate = lastModificationDate;
        this.modificationStatus = modificationStatus;
        this.modifiedOnClient = modifiedOnClient;

        return;
    }

    private MovieRatingData() {}

    public String getMovieId() {
        return movieId;
    } 

    public int getStarNumber() {
        return starNumber;
    } 

    public void setStarNumber(int starNumber) {
        this.starNumber = starNumber;

        return;
    } 

    public long getLastViewingDate() {
        return lastViewingDate;
    } 

    public void setLastViewingDate(long viewingDate) {
        this.lastViewingDate = lastViewingDate;

        return;
    } 

    public long getLastModificationDate() {
        return lastModificationDate;
    } 

    public int getModificationStatus() {
        return modificationStatus;
    } 

    public void setModificationStatus(int modificationStatus) {
        // Should assert the validity of modificationStatus
        this.modificationStatus = modificationStatus;

        return;
    } 

    public boolean isModifiedOnClient() {
        return modifiedOnClient;
    } 

    public void alignTime(long timeShift) {
        lastModificationDate += timeShift;

        return;
    } 

    public String toString() {
        return modifiedOnClient + ":" + modificationStatus + ":" 
               + lastModificationDate + ":" + movieId + ":" + starNumber 
               + ":" + lastViewingDate;
    } 

}

