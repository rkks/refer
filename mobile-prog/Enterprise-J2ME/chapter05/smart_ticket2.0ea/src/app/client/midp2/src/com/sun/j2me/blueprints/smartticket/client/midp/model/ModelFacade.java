

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
 * $Id: ModelFacade.java,v 1.22 2003/03/26 00:01:40 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.model;

import java.util.*;
import java.io.*;
import com.sun.j2me.blueprints.smartticket.client.midp.util.ProgressObserver;
import com.sun.j2me.blueprints.smartticket.shared.midp.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan.Seat;

public class ModelFacade {
    private static final String BASE_NAME_ERROR_RESOURCES = "ErrorResources";
    private static final String L10N_ROOT_DIR = "/l10n/";
    private String locale;
    private static final int[] errorMessageCodeMap = new int[] {
        ErrorMessageCodes.ERROR_GENERAL,    // ModelException.CAUSE_GENERAL
        ErrorMessageCodes.ERROR_UNAVAILABLE_SEATS,    // ModelException.CAUSE_UNAVAILABLE_SEATS
        ErrorMessageCodes.ERROR_USER_ALREADY_EXISTS,    // ModelException.CAUSE_USER_ALREADY_EXISTS
        ErrorMessageCodes.ERROR_USER_NOT_FOUND,    // ModelException.CAUSE_USER_NOT_FOUND
        ErrorMessageCodes.ERROR_INCORRECT_PASSWORD,    // ModelException.CAUSE_INCORRECT_PASSWORD
        ErrorMessageCodes.ERROR_GENERAL,    // ModelException.CAUSE_USER_NOT_LOGGED_IN
        ErrorMessageCodes.ERROR_GENERAL,    // ModelException.CAUSE_THEATERS_NOT_FOUND
        ErrorMessageCodes.ERROR_GENERAL,    // ModelException.CAUSE_THEATER_SCHEDULE_NOT_FOUND
        ErrorMessageCodes.ERROR_GENERAL,    // ModelException.CAUSE_SHOW_TIMES_NOT_FOUND
        ErrorMessageCodes.ERROR_GENERAL,    // ModelException.CAUSE_SHOW_NOT_FOUND
        ErrorMessageCodes.ERROR_GENERAL,    // ModelException.CAUSE_SEATING_PLAN_NOT_FOUND
        ErrorMessageCodes.ERROR_GENERAL,    // ModelException.CAUSE_RESERVATION_NOT_FOUND
        ErrorMessageCodes.ERROR_GENERAL,    // ModelException.CAUSE_MOVIE_NOT_FOUND
        ErrorMessageCodes.ERROR_GENERAL    // ModelException.CAUSE_MOVIE_POSTER_NOT_FOUND
    };
    private boolean isLoggedIn = false;

    /**
     * @link aggregationByValue
     */
    private SynchronizationAgent syncAgent;

    /**
     * @link aggregationByValue
     */
    private RemoteModelProxy remoteModel;

    /**
     * @link aggregationByValue
     */
    private LocalModel localModel;

    public ModelFacade(String serviceURL, 
                       String locale) throws ApplicationException {
        localModel = new LocalModel();
        remoteModel = new RemoteModelProxy(serviceURL);
        syncAgent = new SynchronizationAgent(localModel, remoteModel);

        remoteModel.setPreferences(localModel.getPreferences());
        setLocale(locale, false);

        return;
    }

    public void setProgressObserver(ProgressObserver progressObserver) {
        localModel.setProgressObserver(progressObserver);
        remoteModel.setProgressObserver(progressObserver);
    } 

    public void init() throws ApplicationException {
        localModel.init();
        remoteModel.init();

        return;
    } 

    public void destroy() throws ApplicationException {
        localModel.destroy();
        remoteModel.destroy();

        return;
    } 

    public boolean isOffline() throws ApplicationException {
        return remoteModel.isOffline();
    } 

    public void setOffline(boolean offline) throws ApplicationException {
        remoteModel.setOffline(offline);

        return;
    } 

    public void deleteTheater(String theaterKey) throws ApplicationException {
        localModel.deleteTheater(theaterKey);

        return;
    } 

    public AccountInfo getAccountInfo() throws ApplicationException {
        return localModel.getAccountInfo();
    } 

    public void setAccountInfo(AccountInfo accountInfo) 
            throws ApplicationException {
        localModel.setAccountInfo(accountInfo);

        return;
    } 

    public Preferences getPreferences() throws ApplicationException {
        return localModel.getPreferences();
    } 

    public void setPreferences(Preferences preferences) 
            throws ApplicationException {
        localModel.setPreferences(preferences);

        return;
    } 

    public MovieRating[] getMovieRatings() throws ApplicationException {
        return localModel.getMovieRatings();
    } 

    public void setMovieRating(MovieRating movieRating) 
            throws ApplicationException {
        localModel.setMovieRating(movieRating);

        return;
    } 

    public boolean isLoggedIn() throws ApplicationException {
        return !isOffline() && isLoggedIn;
    } 

    public void login(String userName, 
                      String password) throws ApplicationException {
        try {
            remoteModel.login(userName, password);

            isLoggedIn = true;
        } catch (ModelException me) {
            isLoggedIn = false;

            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 

        return;
    } 

    public void createAccount(AccountInfo accountInfo) 
            throws ApplicationException {
        try {
            remoteModel.createAccount(accountInfo);
        } catch (ModelException me) {
            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 

        localModel.setAccountInfo(accountInfo);

        return;
    } 

    public void updateAccount(AccountInfo accountInfo) 
            throws ApplicationException {
        try {
            remoteModel.updateAccount(accountInfo);
        } catch (ModelException me) {
            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 

        localModel.setAccountInfo(accountInfo);

        return;
    } 

    public Theater[] getTheaters(String zipCode) throws ApplicationException {
        try {
            return remoteModel.getTheaters(zipCode);
        } catch (ModelException me) {
            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 
    } 

    public Theater[] getCachedTheaters(String zipCode) 
            throws ApplicationException {
        return localModel.getCachedTheaters(zipCode);
    } 

    public Theater[] getPersistedTheaters(String zipCode) 
            throws ApplicationException {
        return localModel.getPersistedTheaters(zipCode);
    } 

    public Theater[] getCachedAndPersistedTheaters(String zipCode) 
            throws ApplicationException {
        return localModel.getTheaters(zipCode);
    } 

    public void cleanup() throws ApplicationException {
        localModel.cleanup();

        return;
    } 

    public void persistTheaterSchedule(Theater theater) 
            throws ApplicationException {
        try {
            remoteModel.downloadTheaterSchedule(theater);
        } catch (ModelException me) {
            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 

        return;
    } 

    public Reservation reserveSeats(String theaterKey, String movieKey, 
                                    int[] showTime, 
                                    Seat[] seats) throws ApplicationException {
        try {
            return remoteModel.reserveSeats(theaterKey, movieKey, showTime, 
                                            seats);
        } catch (ModelException me) {
            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 
    } 

    public void purchaseTickets(Reservation reservation) 
            throws ApplicationException {
        try {
            remoteModel.purchaseTickets(reservation.getId());
            localModel.addMovieRating(new MovieRating(remoteModel.getMovie(reservation.getMovieId()), 
                                                      reservation.getShowTime()));
        } catch (ModelException me) {
            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 

        return;
    } 

    public void cancelSeatReservation(Reservation reservation) 
            throws ApplicationException {
        try {
            remoteModel.cancelSeatReservation(reservation.getId());
        } catch (ModelException me) {
            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 

        return;
    } 

    public void synchronizeMovieRatings(int conflictResolutionStrategyId) 
            throws ApplicationException {
        try {
            syncAgent.synchronizeMovieRatings(conflictResolutionStrategyId);

            return;
        } catch (ModelException me) {
            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 
    } 

    public MovieRating[][] synchronizeMovieRatings() 
            throws ApplicationException {
        try {
            return syncAgent.synchronizeMovieRatings();
        } catch (ModelException me) {
            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 
    } 

    public void commitMovieRatings(MovieRating[] resolvedMovieRatings) 
            throws ApplicationException {
        try {
            syncAgent.commitMovieRatings(resolvedMovieRatings);
        } catch (ModelException me) {
            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 

        return;
    } 

    public String[] getLocales() throws ApplicationException {
        try {
            return remoteModel.getLocales();
        } catch (ModelException me) {
            throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
        } 
    } 

    public void setLocale(String locale) throws ApplicationException {
        setLocale(locale, true);

        return;
    } 

    public void setLocale(String locale, 
                          boolean overwrite) throws ApplicationException {
        IndexedResourceBundle errorBundle;

        if (!overwrite) {
            errorBundle = 
                localModel.getResourceBundle(BASE_NAME_ERROR_RESOURCES);

            if (errorBundle != null) {
                this.locale = errorBundle.getLocale();
            } else {
                this.locale = locale;
            } 
        } else {
            this.locale = locale;
        } 

        errorBundle = getResourceBundle(BASE_NAME_ERROR_RESOURCES);

        ApplicationException.setResourceBundle(errorBundle);

        return;
    } 

    public IndexedResourceBundle getResourceBundle(String baseName) 
            throws ApplicationException {
        IndexedResourceBundle resourceBundle = 
            localModel.getResourceBundle(baseName);

        if (resourceBundle == null 
                ||!resourceBundle.getLocale().equals(locale)) {
            try {
                InputStream stream = 
                    getClass().getResourceAsStream(makeResourceBundleName(baseName, 
                        locale));

                resourceBundle = 
                    stream != null 
                    ? IndexedResourceBundle.getBundleFromPropertyFile(locale, stream) 
                    : null;
            } catch (IOException ioe) {
                resourceBundle = null;
            } 

            if (resourceBundle == null) {
                try {
                    resourceBundle = remoteModel.getResourceBundle(baseName, 
                            locale);
                } catch (ModelException me) {
                    throw new ApplicationException(errorMessageCodeMap[me.getCauseCode()]);
                } 
            } 

            localModel.setResourceBundle(baseName, resourceBundle);
        } 

        return resourceBundle;
    } 

    private String makeResourceBundleName(String baseName, String locale) {
        return new StringBuffer(L10N_ROOT_DIR).append(baseName).append("_").append(locale).append(".properties").toString();
    } 

}

