

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
 * $Id: SynchronizationAgent.java,v 1.4 2003/03/20 01:46:52 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.model;

import java.util.*;
import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;

public class SynchronizationAgent {

    /**
     * @link aggregationByValue
     */
    private RemoteModel remoteModel;

    /**
     * @link aggregationByValue
     */
    private LocalModel localModel;
    private SyncAnchor nextSyncAnchor;

    public SynchronizationAgent(LocalModel localModel, RemoteModel remoteModel) 
            throws ApplicationException {
        this.localModel = localModel;
        this.remoteModel = remoteModel;

        return;
    }

    public void synchronizeMovieRatings(int conflictResolutionStrategyId) 
            throws ModelException, ApplicationException {
        SyncAnchor lastSyncAnchor = localModel.getMovieRatingSyncAnchor();

        nextSyncAnchor = remoteModel.initiateSynchronization(lastSyncAnchor 
                != null ? lastSyncAnchor : new SyncAnchor(), 
                        new Date().getTime());

        MovieRating[] movieRatings = localModel.getMovieRatings();
        Vector changes = new Vector();

        // filter out the unchanged movie ratings
        for (int i = 0; i < movieRatings.length; i++) {
            if (nextSyncAnchor.isOutOfSync() 
                    || movieRatings[i].getModificationStatus() 
                       != Synchronizable.STATUS_UNCHANGED) {
                changes.addElement(movieRatings[i]);
            } 
        } 

        MovieRating[] changedMovieRatings = new MovieRating[changes.size()];

        changes.copyInto(changedMovieRatings);

        MovieRating[] updatedMovieRatings = 
            remoteModel.synchronizeMovieRatings(changedMovieRatings, 
                                                conflictResolutionStrategyId);
        // consolidate the updated movie ratings and the initial movie ratings
        Hashtable consolidatedMovieRatings = new Hashtable();

        for (int i = 0; i < movieRatings.length; i++) {
            consolidatedMovieRatings.put(movieRatings[i].getMovie().getPrimaryKey(), 
                                         movieRatings[i]);
        } 

        for (int i = 0; i < updatedMovieRatings.length; i++) {
            consolidatedMovieRatings.put(updatedMovieRatings[i].getMovie().getPrimaryKey(), 
                                         updatedMovieRatings[i]);
        } 

        for (Enumeration elements = consolidatedMovieRatings.elements(); 
                elements.hasMoreElements(); ) {
            MovieRating movieRating = (MovieRating) elements.nextElement();

            if (movieRating.getModificationStatus() 
                    != Synchronizable.STATUS_DELETED) {
                movieRating.setModificationStatus(Synchronizable.STATUS_UNCHANGED);
                localModel.setMovieRating(movieRating);
            } else {
                localModel.purgeMovieRating(movieRating.getPrimaryKey());
            } 
        } 

        localModel.setMovieRatingSyncAnchor(nextSyncAnchor);

        return;
    } 

    public MovieRating[][] synchronizeMovieRatings() 
            throws ModelException, ApplicationException {
        SyncAnchor lastSyncAnchor = localModel.getMovieRatingSyncAnchor();

        nextSyncAnchor = remoteModel.initiateSynchronization(lastSyncAnchor 
                != null ? lastSyncAnchor : new SyncAnchor(), 
                        new Date().getTime());

        MovieRating[] movieRatings = localModel.getMovieRatings();
        Hashtable changes = new Hashtable();

        // filter out the unchanged movie ratings
        for (int i = 0; i < movieRatings.length; i++) {
            if (nextSyncAnchor.isOutOfSync() 
                    || movieRatings[i].getModificationStatus() 
                       != Synchronizable.STATUS_UNCHANGED) {
                changes.put(movieRatings[i].getMovie().getPrimaryKey(), 
                            movieRatings[i]);
            } 
        } 

        MovieRating[] changedMovieRatings = new MovieRating[changes.size()];
        Enumeration elements = changes.elements();

        for (int i = 0; i < changedMovieRatings.length; i++) {
            changedMovieRatings[i] = (MovieRating) elements.nextElement();
        } 

        MovieRating[] conflictingMovieRatings = 
            remoteModel.synchronizeMovieRatings(changedMovieRatings, 
                                                Synchronizable.STRATEGY_ASK_USER);
        MovieRating[][] conflictingMovieRatingPairs = 
            new MovieRating[conflictingMovieRatings.length][2];

        for (int i = 0; i < conflictingMovieRatings.length; i++) {
            conflictingMovieRatingPairs[i][0] = 
                (MovieRating) changes.get(conflictingMovieRatings[i].getMovie().getPrimaryKey());
            conflictingMovieRatingPairs[i][1] = conflictingMovieRatings[i];
        } 

        return conflictingMovieRatingPairs;
    } 

    public void commitMovieRatings(MovieRating[] resolvedMovieRatings) 
            throws ModelException, ApplicationException {
        MovieRating[] movieRatings = localModel.getMovieRatings();
        MovieRating[] updatedMovieRatings = 
            remoteModel.commitMovieRatings(resolvedMovieRatings);
        // consolidate the updated movie ratings and the initial movie ratings
        Hashtable consolidatedMovieRatings = new Hashtable();

        for (int i = 0; i < movieRatings.length; i++) {
            consolidatedMovieRatings.put(movieRatings[i].getMovie().getPrimaryKey(), 
                                         movieRatings[i]);
        } 

        for (int i = 0; i < updatedMovieRatings.length; i++) {
            consolidatedMovieRatings.put(updatedMovieRatings[i].getMovie().getPrimaryKey(), 
                                         updatedMovieRatings[i]);
        } 

        for (Enumeration elements = consolidatedMovieRatings.elements(); 
                elements.hasMoreElements(); ) {
            MovieRating movieRating = (MovieRating) elements.nextElement();

            if (movieRating.getModificationStatus() 
                    != Synchronizable.STATUS_DELETED) {
                movieRating.setModificationStatus(Synchronizable.STATUS_UNCHANGED);
                localModel.setMovieRating(movieRating);
            } else {
                localModel.purgeMovieRating(movieRating.getPrimaryKey());
            } 
        } 

        localModel.setMovieRatingSyncAnchor(nextSyncAnchor);

        return;
    } 

}

