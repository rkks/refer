

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
 * $Id: TicketingBean.java,v 1.6 2003/03/24 22:43:11 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.ejb.ticketing;

import javax.ejb.*;
import javax.naming.*;
import java.rmi.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.reservation.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.schedule.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.show.Seat;

/**
 * @ejbLocal <{com.sun.j2me.blueprints.smartticket.server.ejb.ticketing.TicketingLocal}>
 * @ejbLocalHome <{com.sun.j2me.blueprints.smartticket.server.ejb.ticketing.TicketingLocalHome}>
 * @ejbStateful
 * @undefined
 * @displayName TicketingSB
 * @ejbDontSynchronizeNames
 * @ejbJNDIname ejb/smartticket/smartticket/Ticketing
 * @ejbLocalJNDIname ejb/smartticket/smartticket/Ticketing
 * @ejbTransactionAttribute Required
 */
public class TicketingBean implements SessionBean {
    private SessionContext ctx;
    private Context context;
    private AccountLocal account;
    private ReservationLocal pendingReservation;

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @EJBLink ReservationBean
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.reservation.ReservationLocal
     * @REF_JNDI_NAME ejb/smartticket/smartticket/Reservation
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.reservation.ReservationLocalHome
     * @undefined
     */
    private static final String EJB_REF_RESERVATION = "ejb/Reservation";

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @EJBLink MovieScheduleBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/MovieSchedule
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.schedule.MovieScheduleLocal
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.schedule.MovieScheduleLocalHome
     * @undefined
     */
    private static final String EJB_REF_MOVIE_SCHEDULE = "ejb/MovieSchedule";

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @EJBLink AccountBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/Account
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocal
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocalHome
     * @undefined
     */
    private static final String EJB_REF_ACCOUNT = "ejb/Account";

    public void setSessionContext(SessionContext context) 
            throws RemoteException, EJBException {
        ctx = context;
    } 

    public void ejbActivate() throws EJBException {}

    public void ejbPassivate() throws EJBException {}

    public void ejbRemove() throws EJBException {
        if (pendingReservation != null) {
            pendingReservation.getShow().unbookSeats(pendingReservation.getSeats());

            try {
                pendingReservation.remove();
            } catch (RemoveException re) {
                throw new EJBException("TicketingBean.ejbRemove(): " 
                                       + re.getMessage());
            } 
        } 

        return;
    } 

    public void ejbCreate(AccountLocal account) 
            throws CreateException, EJBException {
        this.account = account;

        try {
            context = (Context) new InitialContext().lookup("java:comp/env");
        } catch (Exception e) {
            throw new CreateException("TicketingBean.ejbCreate(): " 
                                      + e.getMessage());
        } 
    } 

    public ReservationData reserveSeats(String theaterId, String movieId, 
                                        ShowTime showTime, Seat[] seats) {
        try {
            if (pendingReservation != null) {
                pendingReservation.getShow().unbookSeats(pendingReservation.getSeats());

                try {
                    pendingReservation.remove();
                } catch (RemoveException re) {
                    throw new EJBException("TicketingBean.ejbRemove(): " 
                                           + re.getMessage());
                } 
            } 

            MovieScheduleLocalHome movieScheduleHome = 
                (MovieScheduleLocalHome) context.lookup(EJB_REF_MOVIE_SCHEDULE);
            MovieShowLocal movieShow = 
                movieScheduleHome.findByPrimaryKey(new MovieSchedulePK(theaterId, 
                    movieId)).getShow(showTime);

            if (movieShow.bookSeats(seats)) {
                ReservationLocalHome reservationHome = 
                    (ReservationLocalHome) context.lookup(EJB_REF_RESERVATION);

                pendingReservation = reservationHome.create(movieShow, seats);

                return pendingReservation.getData();
            } 

            pendingReservation = null;

            return null;
        } catch (Exception e) {
            throw new EJBException("TicketingBean.reserveSeats(): " 
                                   + e.getMessage());
        } 
    } 

    public void cancelSeatReservation(String reservationId) {
        if (pendingReservation != null) {
            pendingReservation.getShow().unbookSeats(pendingReservation.getSeats());

            try {
                pendingReservation.remove();

                pendingReservation = null;
            } catch (RemoveException re) {
                throw new EJBException("TicketingBean.ejbRemove(): " 
                                       + re.getMessage());
            } 
        } 

        return;
    } 

    public void purchaseTickets(String reservationId) {
        if (pendingReservation != null) {
            try {
                account.getReservations().add(pendingReservation);
                pendingReservation.getShow().confirmBookedSeats(pendingReservation.getSeats());

                pendingReservation = null;

                return;
            } catch (Exception e) {
                throw new EJBException("TicketingBean.purchaseTickets(): " 
                                       + e.getMessage());
            } 
        } 

        throw new EJBException("TicketingBean.purchaseTickets(): No seats previously booked.");
    } 

}

