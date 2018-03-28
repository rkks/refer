

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
 * $Id: SmartTicketFacadeBean.java,v 1.11 2003/03/24 22:43:10 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.ejb;

import javax.ejb.*;
import javax.naming.*;
import java.rmi.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocalHome;
import com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocalHome;
import com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingData;
import com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.reservation.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.schedule.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.show.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.theater.TheaterLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.theater.TheaterLocalHome;
import com.sun.j2me.blueprints.smartticket.server.ejb.ticketing.TicketingLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.ticketing.TicketingLocalHome;

/**
 * @ejbLocal <{SmartTicketFacadeLocal}>
 * @ejbLocalHome <{SmartTicketFacadeLocalHome}>
 * @ejbStateful
 * @ejbJNDIname ejb/smartticket/smartticket/SmartTicketFacade
 * @ejbLocalJNDIname ejb/smartticket/smartticket/SmartTicketFacade
 * @displayName SmartTicketFacadeSB
 * @undefined
 * @ejbDontSynchronizeNames
 * @ejbTransactionAttribute Required
 */
public class SmartTicketFacadeBean implements SessionBean {

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocal
     * @EJBLink AccountBean
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocalHome
     * @REF_JNDI_NAME ejb/smartticket/smartticket/Account
     * @undefined
     */
    private static final String EJB_REF_ACCOUNT = "ejb/Account";

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.theater.TheaterLocal
     * @EJBLink TheaterBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/Theater
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.theater.TheaterLocalHome
     * @undefined
     */
    private static final String EJB_REF_THEATER = "ejb/Theater";

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.schedule.MovieScheduleLocal
     * @EJBLink MovieScheduleBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/MovieSchedule
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.schedule.MovieScheduleLocalHome
     * @undefined
     */
    private static final String EJB_REF_MOVIE_SCHEDULE = "ejb/MovieSchedule";

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowLocal
     * @EJBLink MovieShowBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/MovieShow
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowLocalHome
     * @undefined
     */
    private static final String EJB_REF_MOVIE_SHOW = "ejb/MovieShow";

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal
     * @EJBLink MovieBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/Movie
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocalHome
     * @undefined
     */
    private static final String EJB_REF_MOVIE = "ejb/Movie";

    /**
     * @EJB-REF
     * @ReferenceType Session
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.ticketing.TicketingLocal
     * @EJBLink TicketingBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/Ticketing
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.ticketing.TicketingLocalHome
     * @undefined
     */
    private static final String EJB_REF_TICKETING = "ejb/Ticketing";

    /**
     * @EJB-REF
     * @ReferenceType Session
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SynchronizingLocal
     * @EJBLink SynchronizingBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/Synchronizing
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SynchronizingLocalHome
     * @undefined
     */
    private static final String EJB_REF_SYNCHRONIZING = "ejb/Synchronizing";
    private SessionContext ctx;
    private AccountLocalHome accountHome;
    private TheaterLocalHome theaterHome;
    private MovieScheduleLocalHome movieScheduleHome;
    private MovieShowLocalHome movieShowHome;
    private MovieLocalHome movieHome;
    private TicketingLocalHome ticketingHome;
    private SynchronizingLocalHome synchronizingHome;
    private TicketingLocal ticketing = null;
    private SynchronizingLocal synchronizing = null;
    private AccountLocal account = null;

    /**
     * @link dependency
     */
    /* # TicketingBean lnkTicketingBean; */

    /**
     * @link dependency
     */
    /* # SynchronizingBean lnkSynchronizingBean; */

    /**
     * @link dependency
     */
    /* # AccountBean lnkAccountBean; */

    /**
     * @link dependency
     */
    /* # TheaterBean lnkTheaterBean; */
    public void setSessionContext(SessionContext context) 
            throws RemoteException, EJBException {
        ctx = context;
    } 

    public void ejbActivate() throws EJBException {}

    public void ejbPassivate() throws EJBException {}

    public void ejbRemove() throws EJBException {}

    public void ejbCreate() throws CreateException {
        try {
            Context context = 
                (Context) new InitialContext().lookup("java:comp/env");

            accountHome = (AccountLocalHome) context.lookup(EJB_REF_ACCOUNT);
            theaterHome = (TheaterLocalHome) context.lookup(EJB_REF_THEATER);
            movieScheduleHome = 
                (MovieScheduleLocalHome) context.lookup(EJB_REF_MOVIE_SCHEDULE);
            movieShowHome = 
                (MovieShowLocalHome) context.lookup(EJB_REF_MOVIE_SHOW);
            movieHome = (MovieLocalHome) context.lookup(EJB_REF_MOVIE);
            ticketingHome = 
                (TicketingLocalHome) context.lookup(EJB_REF_TICKETING);
            synchronizingHome = 
                (SynchronizingLocalHome) context.lookup(EJB_REF_SYNCHRONIZING);
        } catch (NamingException ne) {
            throw new CreateException("SmartTicketFacadeBean.ejbCreate(): " 
                                      + ne.getMessage());
        } 

        return;
    } 

    public void login(String userName, 
                      String password) throws SmartTicketFacadeException {
        if (account == null ||!account.getUserName().equals(userName) 
                ||!account.getPassword().equals(password)) {
            try {
                AccountLocal account = 
                    (AccountLocal) accountHome.findByPrimaryKey(userName);

                if (account.getPassword().equals(password)) {
                    this.account = account;

                    try {
                        ticketing = ticketingHome.create(account);
                        synchronizing = synchronizingHome.create(account);

                        return;
                    } catch (CreateException ce) {
                        this.account = null;
                        ticketing = null;
                        synchronizing = null;

                        throw new EJBException("SmartTicketFacadeBean.login: " 
                                               + ce.getMessage());
                    } 
                } 

                throw new SmartTicketFacadeException("Incorrect password.");
            } catch (FinderException fe) {
                throw new SmartTicketFacadeException("No such user.");
            } 
        } 
    } 

    public void createAccount(String userName, String password, 
                              String firstName, String lastName, 
                              String zipCode, String eMail, 
                              String creditCardHolderName, 
                              String CreditCardNumber, 
                              String creditCardExpirationDate, 
                              String creditCardType) throws SmartTicketFacadeException {
        try {
            account = accountHome.create(userName, password);

            account.setFirstName(firstName);
            account.setLastName(lastName);
            account.setZipCode(zipCode);
            account.setEMail(eMail);
            account.setCreditCardNumber(CreditCardNumber);
            account.setCreditCardHolderName(creditCardHolderName);
            account.setCreditCardExpirationDate(creditCardExpirationDate);
            account.setCreditCardType(creditCardType);

            ticketing = ticketingHome.create(account);
            synchronizing = synchronizingHome.create(account);

            return;
        } catch (CreateException ce) {
            ticketing = null;
            synchronizing = null;

            throw new SmartTicketFacadeException("Account creation error.");
        } 
    } 

    public void updateAccount(String userName, String password, 
                              String firstName, String lastName, 
                              String zipCode, String eMail, 
                              String creditCardHolderName, 
                              String CreditCardNumber, 
                              String creditCardExpirationDate, 
                              String creditCardType) throws SmartTicketFacadeException {
        if (account != null && account.getUserName().equals(userName)) {
            account.setPassword(password);
            account.setFirstName(firstName);
            account.setLastName(lastName);
            account.setZipCode(zipCode);
            account.setEMail(eMail);
            account.setCreditCardNumber(CreditCardNumber);
            account.setCreditCardHolderName(creditCardHolderName);
            account.setCreditCardExpirationDate(creditCardExpirationDate);
            account.setCreditCardType(creditCardType);

            return;
        } 

        throw new SmartTicketFacadeException("User not logged in.");
    } 

    public TheaterLocal[] getTheaters(String zipCode) 
            throws SmartTicketFacadeException {
        try {
            return (TheaterLocal[]) new ArrayList(theaterHome.findByZipCode(zipCode)).toArray(new TheaterLocal[0]);
        } catch (FinderException fe) {
            throw new SmartTicketFacadeException("No matching theaters.");
        } 
    } 

    public MovieScheduleLocal[] getMovieSchedules(String theaterId) 
            throws SmartTicketFacadeException {
        try {
            return (MovieScheduleLocal[]) new ArrayList(theaterHome.findByPrimaryKey(theaterId).getSchedule().getMovieSchedules()).toArray(new MovieScheduleLocal[0]);
        } catch (FinderException fe) {
            throw new SmartTicketFacadeException("No matching movie schedules.");
        } 
    } 

    public MovieLocal getMovie(String movieId) 
            throws SmartTicketFacadeException {
        try {
            return movieHome.findByPrimaryKey(movieId);
        } catch (FinderException fe) {
            throw new SmartTicketFacadeException("No matching movie.");
        } 
    } 

    public ShowTimePattern[] getMovieShowTimes(String theaterId, String movieId) 
            throws SmartTicketFacadeException {
        try {
            return movieScheduleHome.findByPrimaryKey(new MovieSchedulePK(theaterId, 
                    movieId)).getShowTimePatterns();
        } catch (FinderException fe) {
            throw new SmartTicketFacadeException("No matching movie schedule.");
        } 
    } 

    public Seating getSeating(String theaterId, String movieId, ShowTime showTime) 
            throws SmartTicketFacadeException {
        try {
            return movieScheduleHome.findByPrimaryKey(new MovieSchedulePK(theaterId, 
                    movieId)).getShow(showTime).getSeating();
        } catch (FinderException fe) {
            throw new SmartTicketFacadeException("No matching movie show.");
        } 
    } 

    public ReservationData reserveSeats(String theaterId, String movieId, 
                                        ShowTime showTime, 
                                        Seat[] seats) throws SmartTicketFacadeException {
        if (ticketing != null) {
            return ticketing.reserveSeats(theaterId, movieId, showTime, 
                                          seats);
        } 

        throw new SmartTicketFacadeException("User not logged in.");
    } 

    public void purchaseTickets(String reservationId) 
            throws SmartTicketFacadeException {
        if (ticketing != null) {
            ticketing.purchaseTickets(reservationId);

            return;
        } 

        throw new SmartTicketFacadeException("User not logged in.");
    } 

    public void cancelSeatReservation(String reservationId) 
            throws SmartTicketFacadeException {
        if (ticketing != null) {
            ticketing.cancelSeatReservation(reservationId);

            return;
        } 

        throw new SmartTicketFacadeException("User not logged in.");
    } 

    public SyncClientAnchor initiateSynchronization(SyncClientAnchor syncClientAnchor, 
            long clientTime) throws SmartTicketFacadeException {
        if (synchronizing != null) {
            return synchronizing.initiateSynchronization(syncClientAnchor, 
                    clientTime);
        } 

        throw new SmartTicketFacadeException("User not logged in.");
    } 

    public MovieRatingData[] synchronizeMovieRatings(MovieRatingData[] movieRatings, 
            int conflictResolutionStrategyId) throws SmartTicketFacadeException {
        if (synchronizing != null) {
            return synchronizing.synchronizeMovieRatings(movieRatings, 
                    conflictResolutionStrategyId);
        } 

        throw new SmartTicketFacadeException("User not logged in.");
    } 

    public MovieRatingData[] commitMovieRatings(MovieRatingData[] movieRatings) 
            throws SmartTicketFacadeException {
        if (synchronizing != null) {
            return synchronizing.commitMovieRatings(movieRatings);
        } 

        throw new SmartTicketFacadeException("User not logged in.");
    } 

    public MovieRatingLocal[] getMovieRatings() 
            throws SmartTicketFacadeException {
        if (account != null) {
            return (MovieRatingLocal[]) new ArrayList(account.getMovieRatings()).toArray(new MovieRatingLocal[0]);
        } 

        throw new SmartTicketFacadeException("User not logged in.");
    } 

    public MovieRatingLocal getMovieRating(String movieId) 
            throws SmartTicketFacadeException {
        if (account != null) {
            return account.getMovieRating(movieId);
        } 

        throw new SmartTicketFacadeException("User not logged in.");
    } 

}

