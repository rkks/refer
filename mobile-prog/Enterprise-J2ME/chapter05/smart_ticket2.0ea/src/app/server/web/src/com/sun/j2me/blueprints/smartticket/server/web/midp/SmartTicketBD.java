

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
 * $Id: SmartTicketBD.java,v 1.13 2003/03/19 22:59:32 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.web.midp;

import javax.ejb.*;
import javax.naming.*;
import javax.rmi.*;
import javax.servlet.*;
import java.io.*;
import java.rmi.*;
import java.util.*;
import java.net.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.account.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.movie.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.schedule.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.reservation.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.show.Seat;
import com.sun.j2me.blueprints.smartticket.server.ejb.show.Seating;
import com.sun.j2me.blueprints.smartticket.server.ejb.theater.TheaterLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingData;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.IndexedResourceBundle;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;

/**
 * This "business delegate" (see "Core J2EE Patterns" for more information on this pattern) provides an interface to the
 * application's business functionality.
 */
public class SmartTicketBD implements RemoteModel {
    public static final String EJB_REF_FACADE = "ejb/SmartTicketFacade";
    private SmartTicketFacadeLocal facade;
    private ServletContext servletContext = null;

    public SmartTicketBD(ServletContext servletContext) 
            throws ApplicationException {
        this.servletContext = servletContext;

        try {
            Context context = 
                (Context) new InitialContext().lookup("java:comp/env");

            facade = 
                ((SmartTicketFacadeLocalHome) context.lookup(EJB_REF_FACADE)).create();

            return;
        } catch (Exception e) {
            throw new ApplicationException(e);
        } 
    }

    protected void clear() {
        try {
            facade.remove();
        } catch (RemoveException re) {}

        return;
    } 

    public void login(String userName, String password) 
            throws ModelException, ApplicationException {
        System.err.println("login: " + userName + " " + password);

        try {
            facade.login(userName, password);

            return;
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_USER_NOT_FOUND);
        } catch (Exception e) {
            throw new ApplicationException(e);
        } 
    } 

    public void createAccount(AccountInfo accountInfo) 
            throws ModelException, ApplicationException {
        try {
            facade.createAccount(accountInfo.getUserName(), 
                                 accountInfo.getPassword(), "", "", "00000", 
                                 "",    /* FIXME */
            accountInfo.getCreditCardNumber(), 
            accountInfo.getCreditCardHolderName(), 
            accountInfo.getCreditCardExpirationDate(), 
            "" /* accountInfo.getCreditCardType() */ /* FIXME */);

            return;
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_USER_ALREADY_EXISTS);
        } catch (Exception e) {
            throw new ApplicationException(e);
        } 
    } 

    public void updateAccount(AccountInfo accountInfo) 
            throws ModelException, ApplicationException {
        try {
            facade.updateAccount(accountInfo.getUserName(), 
                                 accountInfo.getPassword(), "", "", "00000", 
                                 "",    /* FIXME */
            accountInfo.getCreditCardNumber(), 
            accountInfo.getCreditCardHolderName(), 
            accountInfo.getCreditCardExpirationDate(), 
            "" /* accountInfo.getCreditCardType() */ /* FIXME */);

            return;
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_USER_NOT_LOGGED_IN);
        } catch (Exception e) {
            throw new ApplicationException(e);
        } 
    } 

    public Theater[] getTheaters(String zipCode) 
            throws ModelException, ApplicationException {
        try {
            TheaterLocal[] theaters = facade.getTheaters(zipCode);
            Theater[] _theaters = new Theater[theaters.length];

            for (int i = 0; i < _theaters.length; i++) {
                _theaters[i] = new Theater(theaters[i].getId(), 
                                           theaters[i].getName(), 
                                           theaters[i].getAddress(), 
                                           theaters[i].getZipCode());
            } 

            return _theaters;
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_THEATERS_NOT_FOUND);
        } catch (Exception e) {
            e.printStackTrace(System.err);

            throw new ApplicationException(e);
        } 
    } 

    public TheaterSchedule getTheaterSchedule(String theaterKey) 
            throws ModelException, ApplicationException {
        try {
            MovieScheduleLocal[] movieSchedules = 
                facade.getMovieSchedules(theaterKey);
            TheaterSchedule _theaterSchedule = new TheaterSchedule(theaterKey, 
                    movieSchedules[0].getTheaterSchedule().getExpirationDate().getTime());
            TheaterSchedule.MovieSchedule[] _movieSchedules = 
                new TheaterSchedule.MovieSchedule[movieSchedules.length];

            for (int i = 0; i < _movieSchedules.length; i++) {
                _movieSchedules[i] = 
                    _theaterSchedule.new MovieSchedule(new Movie(movieSchedules[i].getMovieId(), movieSchedules[i].getMovie().getTitle(), movieSchedules[i].getMovie().getSummary(), movieSchedules[i].getMovie().getRating()), 
                                                       null);
            } 

            _theaterSchedule.setMovieSchedules(_movieSchedules);

            return _theaterSchedule;
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_THEATER_SCHEDULE_NOT_FOUND);
        } catch (Exception e) {
            e.printStackTrace(System.err);

            throw new ApplicationException(e);
        } 
    } 

    public Movie getMovie(String movieKey) 
            throws ModelException, ApplicationException {
        try {
            MovieLocal movieLocal = facade.getMovie(movieKey);
            Movie movie = new Movie(movieLocal.getId(), 
                                    movieLocal.getTitle(), 
                                    movieLocal.getSummary(), 
                                    movieLocal.getRating());

            return movie;
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_MOVIE_NOT_FOUND);
        } catch (Exception e) {
            throw new ApplicationException(e);
        } 
    } 

    public byte[] getMoviePoster(String movieKey) 
            throws ModelException, ApplicationException {
        try {
            MovieLocal movieLocal = facade.getMovie(movieKey);
            String posterURL = movieLocal.getPosterURL();
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            InputStream in = getResourceAsStream(posterURL);
            byte[] buffer = new byte[1024];

            for (int n = 0; (n = in.read(buffer, 0, buffer.length)) >= 0; ) {
                out.write(buffer, 0, n);
            } 

            return out.toByteArray();
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_MOVIE_POSTER_NOT_FOUND);
        } catch (Exception e) {
            throw new ApplicationException(e);
        } 
    } 

    private InputStream getResourceAsStream(String path) throws IOException {
        try {
            return new URL(path).openStream();
        } catch (MalformedURLException exception) {
            return servletContext.getResourceAsStream(path);
        } 
    } 

    public int[][] getMovieShowTimes(String theaterKey, String movieKey) 
            throws ModelException, ApplicationException {
        try {
            ShowTimePattern[] showTimePatterns = 
                facade.getMovieShowTimes(theaterKey, movieKey);
            int[][] _showTimePatterns = new int[showTimePatterns.length][];

            for (int i = 0; i < _showTimePatterns.length; i++) {
                _showTimePatterns[i] = new int[] {
                    showTimePatterns[i].getDayOfWeek(), 
                    showTimePatterns[i].getHour(), 
                    showTimePatterns[i].getMinute()
                };
            } 

            return _showTimePatterns;
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_SHOW_TIMES_NOT_FOUND);
        } catch (Exception e) {
            e.printStackTrace(System.err);

            throw new ApplicationException(e);
        } 
    } 

    public SeatingPlan getSeatingPlan(String theaterKey, String movieKey, 
                                      int[] showtime) throws ModelException, 
                                      ApplicationException {
        try {
            Seating seating = facade.getSeating(theaterKey, movieKey, 
                                                new ShowTime(showtime[0], 
                                                showtime[1], showtime[2]));

            return new SeatingPlan(seating.getSeatData(), 
                                   seating.getRowLength());
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_SHOW_NOT_FOUND);
        } catch (Exception e) {
            e.printStackTrace(System.err);

            throw new ApplicationException(e);
        } 
    } 

    public Reservation reserveSeats(String theaterKey, String movieKey, 
                                    int[] showtime, 
                                    SeatingPlan.Seat[] _seats) throws ModelException, 
                                    ApplicationException {
        try {
            Seat[] seats = new Seat[_seats.length];

            for (int i = 0; i < seats.length; i++) {
                seats[i] = new Seat(_seats[i].getRowNumber(), 
                                    _seats[i].getSeatNumber());
            } 

            ReservationData reservation = facade.reserveSeats(theaterKey, 
                    movieKey, 
                    new ShowTime(showtime[0], showtime[1], showtime[2]), 
                    seats);

            if (reservation == null) {
                throw new ModelException(ModelException.CAUSE_UNAVAILABLE_SEATS);
            } 

            return new Reservation(reservation.getId(), 
                                   reservation.getMovieId(), 
                                   reservation.getShowTime());
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_USER_NOT_LOGGED_IN);
        } catch (Exception e) {
            e.printStackTrace(System.err);

            throw new ApplicationException(e);
        } 
    } 

    public void purchaseTickets(String reservationId) 
            throws ModelException, ApplicationException {
        try {
            facade.purchaseTickets(reservationId);

            return;
        } catch (SmartTicketFacadeException stfe) {
            stfe.printStackTrace(System.err);

            throw new ModelException(ModelException.CAUSE_USER_NOT_LOGGED_IN);
        } catch (Exception e) {
            e.printStackTrace(System.err);

            throw new ApplicationException(e);
        } 
    } 

    public void cancelSeatReservation(String reservationId) 
            throws ModelException, ApplicationException {
        try {
            facade.cancelSeatReservation(reservationId);

            return;
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_USER_NOT_LOGGED_IN);
        } catch (Exception e) {
            e.printStackTrace(System.err);

            throw new ApplicationException(e);
        } 
    } 

    public String[] getLocales() throws ApplicationException {
        List localeList = new ArrayList();
        String localesString = servletContext.getInitParameter("locales");
        StringTokenizer tokenizer = new StringTokenizer(localesString, ",");

        while (tokenizer.hasMoreTokens()) {
            localeList.add(tokenizer.nextToken());
        } 

        String[] locales = new String[localeList.size()];

        localeList.toArray(locales);

        return locales;
    } 

    public IndexedResourceBundle getResourceBundle(String baseName, 
            String locale) throws ApplicationException {
        try {
            InputStream in = 
                servletContext.getResourceAsStream("/WEB-INF/rsrc/l10n/" 
                                                   + baseName + "_" + locale 
                                                   + ".properties");

            return IndexedResourceBundle.getBundleFromPropertyFile(locale, 
                    in);
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public SyncAnchor initiateSynchronization(SyncAnchor syncAnchor, long clientTime) 
            throws ModelException, ApplicationException {
        try {
            SyncClientAnchor syncClientAnchor = 
                new SyncClientAnchor(syncAnchor.getClientId(), 
                                     syncAnchor.getLastSyncTime());

            syncClientAnchor = 
                facade.initiateSynchronization(syncClientAnchor, clientTime);

            return new SyncAnchor(syncClientAnchor.getClientId(), 
                                  syncClientAnchor.getLastSyncTime(), 
                                  syncClientAnchor.isOutOfSync());
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_USER_NOT_LOGGED_IN);
        } catch (Exception e) {
            e.printStackTrace(System.err);

            throw new ApplicationException(e);
        } 
    } 

    public MovieRating[] synchronizeMovieRatings(MovieRating[] movieRatings, 
            int conflictResolutionStrategyId) throws ModelException, 
            ApplicationException {
        try {
            return movieRatingDataToMovieRatings(facade.synchronizeMovieRatings(movieRatingsToMovieRatingData(movieRatings), 
                    conflictResolutionStrategyId));
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_USER_NOT_LOGGED_IN);
        } catch (Exception e) {
            e.printStackTrace(System.err);

            throw new ApplicationException(e);
        } 
    } 

    public MovieRating[] commitMovieRatings(MovieRating[] movieRatings) 
            throws ModelException, ApplicationException {
        try {
            return movieRatingDataToMovieRatings(facade.commitMovieRatings(movieRatingsToMovieRatingData(movieRatings)));
        } catch (SmartTicketFacadeException stfe) {
            throw new ModelException(ModelException.CAUSE_USER_NOT_LOGGED_IN);
        } catch (Exception e) {
            e.printStackTrace(System.err);

            throw new ApplicationException(e);
        } 
    } 

    private MovieRatingData[] movieRatingsToMovieRatingData(MovieRating[] movieRatings) {
        List movieRatingData = new ArrayList();

        if (movieRatings != null) {
            for (int i = 0; i < movieRatings.length; i++) {
                movieRatingData.add(new MovieRatingData(movieRatings[i].getPrimaryKey(), 
                                                        movieRatings[i].getStarNumber(), 
                                                        movieRatings[i].getLastViewingDate(), 
                                                        movieRatings[i].getLastModificationDate(), 
                                                        movieRatings[i].getModificationStatus(), 
                                                        movieRatings[i].isModifiedOnClient()));
            } 
        } 

        return (MovieRatingData[]) movieRatingData.toArray(new MovieRatingData[0]);
    } 

    private MovieRating[] movieRatingDataToMovieRatings(MovieRatingData[] movieRatingData) {
        List movieRatings = new ArrayList();

        if (movieRatingData != null) {
            for (int i = 0; i < movieRatingData.length; i++) {
                movieRatings.add(new MovieRating(movieRatingData[i].getMovieId(), 
                                                 movieRatingData[i].getStarNumber(), 
                                                 movieRatingData[i].getLastViewingDate(), 
                                                 movieRatingData[i].getLastModificationDate(), 
                                                 movieRatingData[i].getModificationStatus(), 
                                                 movieRatingData[i].isModifiedOnClient()));
            } 
        } 

        return (MovieRating[]) movieRatings.toArray(new MovieRating[0]);
    } 

}

