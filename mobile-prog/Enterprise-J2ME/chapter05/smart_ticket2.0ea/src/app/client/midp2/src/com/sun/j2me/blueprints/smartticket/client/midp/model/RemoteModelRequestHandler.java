

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
 * $Id: RemoteModelRequestHandler.java,v 1.6 2003/03/21 23:19:33 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.model;

import com.sun.j2me.blueprints.smartticket.client.midp.util.ProgressObserver;
import com.sun.j2me.blueprints.smartticket.shared.midp.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan.Seat;

abstract public class RemoteModelRequestHandler implements RequestHandler, 
        RemoteModel {

    /**
     * @link aggregation
     */
    private RemoteModelRequestHandler nextHandler;
    private Preferences preferences;
    protected static ProgressObserver progressObserver;

    public RemoteModelRequestHandler(RemoteModelRequestHandler nextHandler) {
        this.nextHandler = nextHandler;
    }

    public RequestHandler getNextHandler() {
        return nextHandler;
    } 

    public void init() throws ApplicationException {
        if (nextHandler != null) {
            nextHandler.init();
        } 

        return;
    } 

    public void destroy() throws ApplicationException {
        if (nextHandler != null) {
            nextHandler.destroy();
        } 

        return;
    } 

    public static void setProgressObserver(ProgressObserver progressObserver) {
        RemoteModelRequestHandler.progressObserver = progressObserver;
    } 

    protected static void updateProgress() throws ApplicationException {
        if (progressObserver != null) {
            if (!progressObserver.isStopped()) {
                progressObserver.updateProgress();

                return;
            } 

            throw new ApplicationException(ErrorMessageCodes.ERROR_OPERATION_INTERRUPTED);
        } 
    } 

    private RemoteModelRequestHandler getRemoteModelRequestHandler() 
            throws ApplicationException {
        if (nextHandler != null) {
            return nextHandler;
        } 

        throw new ApplicationException(ErrorMessageCodes.ERROR_BROKEN_HANDLER_CHAIN);
    } 

    public void setPreferences(Preferences preferences) {
        this.preferences = preferences;

        if (nextHandler != null) {
            nextHandler.setPreferences(preferences);
        } 

        return;
    } 

    public Preferences getPreferences() {
        return preferences;
    } 

    public boolean isOffline() throws ApplicationException {
        return getRemoteModelRequestHandler().isOffline();
    } 

    public void setOffline(boolean offline) throws ApplicationException {
        getRemoteModelRequestHandler().setOffline(offline);

        return;
    } 

    public void login(String userName, String password) 
            throws ModelException, ApplicationException {
        getRemoteModelRequestHandler().login(userName, password);

        return;
    } 

    public void createAccount(AccountInfo accountInfo) 
            throws ModelException, ApplicationException {
        getRemoteModelRequestHandler().createAccount(accountInfo);

        return;
    } 

    public void updateAccount(AccountInfo accountInfo) 
            throws ModelException, ApplicationException {
        getRemoteModelRequestHandler().updateAccount(accountInfo);

        return;
    } 

    public Theater[] getTheaters(String zipCode) 
            throws ModelException, ApplicationException {
        return getRemoteModelRequestHandler().getTheaters(zipCode);
    } 

    public TheaterSchedule getTheaterSchedule(Theater theater) 
            throws ModelException, ApplicationException {
        return getRemoteModelRequestHandler().getTheaterSchedule(theater);
    } 

    public TheaterSchedule downloadTheaterSchedule(Theater theater) 
            throws ModelException, ApplicationException {
        return getRemoteModelRequestHandler().downloadTheaterSchedule(theater);
    } 

    public TheaterSchedule getTheaterSchedule(String theaterKey) 
            throws ModelException, ApplicationException {
        return getRemoteModelRequestHandler().getTheaterSchedule(theaterKey);
    } 

    public Movie getMovie(String movieKey) 
            throws ModelException, ApplicationException {
        return getRemoteModelRequestHandler().getMovie(movieKey);
    } 

    public byte[] getMoviePoster(String movieKey) 
            throws ModelException, ApplicationException {
        return getRemoteModelRequestHandler().getMoviePoster(movieKey);
    } 

    public int[][] getMovieShowTimes(String theaterKey, String movieKey) 
            throws ModelException, ApplicationException {
        return getRemoteModelRequestHandler().getMovieShowTimes(theaterKey, 
                movieKey);
    } 

    public SeatingPlan getSeatingPlan(String theaterKey, String movieKey, 
                                      int[] showTime) throws ModelException, 
                                      ApplicationException {
        return getRemoteModelRequestHandler().getSeatingPlan(theaterKey, 
                movieKey, showTime);
    } 

    public Reservation reserveSeats(String theaterKey, String movieKey, 
                                    int[] showTime, 
                                    Seat[] seats) throws ModelException, 
                                    ApplicationException {
        return getRemoteModelRequestHandler().reserveSeats(theaterKey, 
                movieKey, showTime, seats);
    } 

    public void purchaseTickets(String reservationId) 
            throws ModelException, ApplicationException {
        getRemoteModelRequestHandler().purchaseTickets(reservationId);
    } 

    public void cancelSeatReservation(String reservationId) 
            throws ModelException, ApplicationException {
        getRemoteModelRequestHandler().cancelSeatReservation(reservationId);
    } 

    public SyncAnchor initiateSynchronization(SyncAnchor syncAnchor, long clientTime) 
            throws ModelException, ApplicationException {
        return getRemoteModelRequestHandler().initiateSynchronization(syncAnchor, 
                clientTime);
    } 

    public MovieRating[] synchronizeMovieRatings(MovieRating[] movieRatings, 
            int conflictResolutionStrategyId) throws ModelException, 
            ApplicationException {
        return getRemoteModelRequestHandler().synchronizeMovieRatings(movieRatings, 
                conflictResolutionStrategyId);
    } 

    public MovieRating[] commitMovieRatings(MovieRating[] movieRatings) 
            throws ModelException, ApplicationException {
        return getRemoteModelRequestHandler().commitMovieRatings(movieRatings);
    } 

    public String[] getLocales() throws ModelException, ApplicationException {
        return getRemoteModelRequestHandler().getLocales();
    } 

    public IndexedResourceBundle getResourceBundle(String baseName, 
            String locale) throws ModelException, ApplicationException {
        return getRemoteModelRequestHandler().getResourceBundle(baseName, 
                locale);
    } 

}

