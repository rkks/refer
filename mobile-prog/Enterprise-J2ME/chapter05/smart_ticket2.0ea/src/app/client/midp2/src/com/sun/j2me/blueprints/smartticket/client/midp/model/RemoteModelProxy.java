

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
 * $Id: RemoteModelProxy.java,v 1.5 2003/03/21 23:19:33 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.model;

import java.util.*;
import com.sun.j2me.blueprints.smartticket.client.midp.util.ProgressObserver;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.IndexedResourceBundle;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan.Seat;

public class RemoteModelProxy extends ModelObjectLoader 
    implements RemoteModel {

    /**
     * @link dependency
     */
    /* # RMSCacheHandler lnkRMSCacheHandler; */
    private RemoteModelRequestHandler requestHandlerChain;
    private Preferences preferences = null;
    private Hashtable movies = new Hashtable();

    public RemoteModelProxy(String serviceURL) throws ApplicationException {
        requestHandlerChain = 
            new RMSCacheHandler(new HTTPCommunicationHandler(null, 
                serviceURL));

        return;
    }

    public void setProgressObserver(ProgressObserver progressObserver) {
        RemoteModelRequestHandler.setProgressObserver(progressObserver);
    } 

    public void init() throws ApplicationException {
        requestHandlerChain.init();

        return;
    } 

    public void destroy() throws ApplicationException {
        requestHandlerChain.destroy();

        return;
    } 

    public boolean isOffline() throws ApplicationException {
        return requestHandlerChain.isOffline();
    } 

    public void setOffline(boolean offline) throws ApplicationException {
        requestHandlerChain.setOffline(offline);

        return;
    } 

    public void setPreferences(Preferences preferences) {
        this.preferences = preferences;

        requestHandlerChain.setPreferences(preferences);

        return;
    } 

    public void login(String userName, String password) 
            throws ModelException, ApplicationException {
        requestHandlerChain.login(userName, password);

        return;
    } 

    public void createAccount(AccountInfo accountInfo) 
            throws ModelException, ApplicationException {
        requestHandlerChain.createAccount(accountInfo);

        return;
    } 

    public void updateAccount(AccountInfo accountInfo) 
            throws ModelException, ApplicationException {
        requestHandlerChain.updateAccount(accountInfo);

        return;
    } 

    public Theater[] getTheaters(String zipCode) 
            throws ModelException, ApplicationException {
        return requestHandlerChain.getTheaters(zipCode);
    } 

    public TheaterSchedule getTheaterSchedule(Theater theater) 
            throws ModelException, ApplicationException {
        return requestHandlerChain.getTheaterSchedule(theater);
    } 

    public TheaterSchedule downloadTheaterSchedule(Theater theater) 
            throws ModelException, ApplicationException {
        return requestHandlerChain.downloadTheaterSchedule(theater);
    } 

    public TheaterSchedule getTheaterSchedule(String theaterKey) 
            throws ModelException, ApplicationException {
        return requestHandlerChain.getTheaterSchedule(theaterKey);
    } 

    public Movie getMovie(String movieKey) 
            throws ModelException, ApplicationException {
        Movie movie = (Movie) movies.get(movieKey);

        if (movie == null) {
            movie = requestHandlerChain.getMovie(movieKey);

            movies.put(movieKey, movie);
        } 

        return movie;
    } 

    public Movie getMovie(Movie movie) 
            throws ModelException, ApplicationException {
        Movie instance = (Movie) movies.get(movie.getPrimaryKey());

        if (instance == null) {
            movies.put(movie.getPrimaryKey(), movie);

            instance = movie;
        } 

        return instance;
    } 

    public byte[] getMoviePoster(String movieKey) 
            throws ModelException, ApplicationException {
        return requestHandlerChain.getMoviePoster(movieKey);
    } 

    public int[][] getMovieShowTimes(String theaterKey, String movieKey) 
            throws ModelException, ApplicationException {
        return requestHandlerChain.getMovieShowTimes(theaterKey, movieKey);
    } 

    public SeatingPlan getSeatingPlan(String theaterKey, String movieKey, 
                                      int[] showTime) throws ModelException, 
                                      ApplicationException {
        return requestHandlerChain.getSeatingPlan(theaterKey, movieKey, 
                                                  showTime);
    } 

    public Reservation reserveSeats(String theaterKey, String movieKey, 
                                    int[] showTime, 
                                    Seat[] seats) throws ModelException, 
                                    ApplicationException {
        return requestHandlerChain.reserveSeats(theaterKey, movieKey, 
                                                showTime, seats);
    } 

    public void purchaseTickets(String reservationId) 
            throws ModelException, ApplicationException {
        requestHandlerChain.purchaseTickets(reservationId);

        return;
    } 

    public void cancelSeatReservation(String reservationId) 
            throws ModelException, ApplicationException {
        requestHandlerChain.cancelSeatReservation(reservationId);

        return;
    } 

    public SyncAnchor initiateSynchronization(SyncAnchor syncAnchor, long clientTime) 
            throws ModelException, ApplicationException {
        return requestHandlerChain.initiateSynchronization(syncAnchor, 
                clientTime);
    } 

    public MovieRating[] synchronizeMovieRatings(MovieRating[] movieRatings, 
            int conflictResolutionStrategyId) throws ModelException, 
            ApplicationException {
        return requestHandlerChain.synchronizeMovieRatings(movieRatings, 
                conflictResolutionStrategyId);
    } 

    public MovieRating[] commitMovieRatings(MovieRating[] movieRatings) 
            throws ModelException, ApplicationException {
        return requestHandlerChain.commitMovieRatings(movieRatings);
    } 

    public String[] getLocales() throws ModelException, ApplicationException {
        return requestHandlerChain.getLocales();
    } 

    public IndexedResourceBundle getResourceBundle(String baseName, 
            String locale) throws ModelException, ApplicationException {
        return requestHandlerChain.getResourceBundle(baseName, locale);
    } 

}

