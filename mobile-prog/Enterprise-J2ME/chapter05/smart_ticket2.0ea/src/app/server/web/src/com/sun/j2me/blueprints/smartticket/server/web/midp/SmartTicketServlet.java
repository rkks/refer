

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
 * $Id: SmartTicketServlet.java,v 1.7 2003/03/19 07:56:52 ro89390 Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.web.midp;

import javax.servlet.*;
import javax.servlet.http.*;
import java.io.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;

/**
 * This class processes an HTTP request and starts decoding the application request it contains.
 */
public class SmartTicketServlet extends HttpServlet {
    public static final String SESSION_ATTRIBUTE_SMART_TICKET_BD = 
        "com.sun.j2me.blueprints.smartticket.server.web.midp.SmartTicketBD";

    protected void doPost(HttpServletRequest request, 
                          HttpServletResponse response) throws ServletException, 
                          IOException {
        HttpSession session = request.getSession(true);
        SmartTicketBD smartTicketBD = 
            (SmartTicketBD) session.getAttribute(SESSION_ATTRIBUTE_SMART_TICKET_BD);

        response.setContentType("application/octet-stream");

        try {
            int contentLength = handleCall(smartTicketBD, 
                                           request.getInputStream(), 
                                           response.getOutputStream());

            response.setContentLength(contentLength);

            if (session.isNew()) {
                response.setStatus(HttpServletResponse.SC_CREATED);
                response.setHeader("Location", 
                                   response.encodeURL(request.getRequestURL().toString()));
            } 
        } catch (ApplicationException ae) {
            ae.printStackTrace();
            response.setStatus(HttpServletResponse.SC_INTERNAL_SERVER_ERROR);
        } 
        finally {
            // FIXME
        } 

        return;
    } 

    public int handleCall(SmartTicketBD smartTicketBD, InputStream in, 
                          OutputStream out) throws IOException, 
                          ApplicationException {
        DataInputStream call = new DataInputStream(in);
        DataOutputStream result = new DataOutputStream(out);

        try {
            ByteArrayOutputStream bufferedResult = 
                new ByteArrayOutputStream();
            DataOutputStream successfulResult = 
                new DataOutputStream(bufferedResult);
            byte method = call.readByte();

            switch (method) {

            case MessageConstants.OPERATION_LOGIN_USER:
                login(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_CREATE_ACCOUNT:
                createAccount(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_UPDATE_ACCOUNT:
                updateAccount(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_GET_THEATERS:
                getTheaters(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_GET_THEATER_SCHEDULE:
                getTheaterSchedule(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_GET_MOVIE:
                getMovie(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_GET_MOVIE_POSTER:
                getMoviePoster(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_GET_MOVIE_SHOWTIMES:
                getMovieShowtimes(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_GET_SEATING_PLAN:
                getSeatingPlan(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_RESERVE_SEATS:
                reserveSeats(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_PURCHASE_TICKETS:
                purchaseTickets(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_CANCEL_SEAT_RESERVATION:
                cancelSeatReservation(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_GET_LOCALES:
                getLocales(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_GET_RESOURCE_BUNDLE:
                getResourceBundle(smartTicketBD, call, successfulResult);

                break;

            case MessageConstants.OPERATION_INITIATE_SYNCHRONIZATION:
                initiateSynchronization(smartTicketBD, call, 
                                        successfulResult);

                break;

            case MessageConstants.OPERATION_SYNCHRONIZE_MOVIE_RATINGS:
                synchronizeMovieRatings(smartTicketBD, call, 
                                        successfulResult);

                break;

            case MessageConstants.OPERATION_COMMIT_MOVIE_RATINGS:
                commitMovieRatings(smartTicketBD, call, successfulResult);

                break;

            default:
                result.writeInt(MessageConstants.ERROR_UNKNOWN_OPERATION);
            }

            successfulResult.flush();

            byte[] bufferedBytes = bufferedResult.toByteArray();

            result.writeInt(MessageConstants.ERROR_NONE);
            result.write(bufferedBytes, 0, bufferedBytes.length);
        } catch (ModelException me) {
            result.writeInt(MessageConstants.ERROR_MODEL_EXCEPTION);
            me.serialize(result);
        } 

        return result.size();
    } 

    private void login(SmartTicketBD smartTicketBD, DataInputStream call, 
                       DataOutputStream result) throws IOException, 
                       ModelException, ApplicationException {
        String username = call.readUTF();
        String password = call.readUTF();

        smartTicketBD.login(username, password);

        return;
    } 

    private void createAccount(SmartTicketBD smartTicketBD, 
                               DataInputStream call, 
                               DataOutputStream result) throws IOException, 
                               ModelException, ApplicationException {
        AccountInfo accountInfo = AccountInfo.deserialize(call);

        smartTicketBD.createAccount(accountInfo);

        return;
    } 

    private void updateAccount(SmartTicketBD smartTicketBD, 
                               DataInputStream call, 
                               DataOutputStream result) throws IOException, 
                               ModelException, ApplicationException {
        AccountInfo accountInfo = AccountInfo.deserialize(call);

        smartTicketBD.updateAccount(accountInfo);

        return;
    } 

    private void getTheaters(SmartTicketBD smartTicketBD, 
                             DataInputStream call, 
                             DataOutputStream result) throws IOException, 
                             ModelException, ApplicationException {
        String zipCode = call.readUTF();
        Theater[] theaters = smartTicketBD.getTheaters(zipCode);

        result.writeInt(theaters.length);

        for (int i = 0; i < theaters.length; i++) {
            theaters[i].serialize(result);
        } 

        return;
    } 

    private void getTheaterSchedule(SmartTicketBD smartTicketBD, 
                                    DataInputStream call, 
                                    DataOutputStream result) throws IOException, 
                                    ModelException, ApplicationException {
        String theaterKey = call.readUTF();
        TheaterSchedule theaterSchedule = 
            smartTicketBD.getTheaterSchedule(theaterKey);

        theaterSchedule.serialize(result, true);

        return;
    } 

    private void getMovie(SmartTicketBD smartTicketBD, DataInputStream call, 
                          DataOutputStream result) throws IOException, 
                          ModelException, ApplicationException {
        String movieKey = call.readUTF();
        Movie movie = smartTicketBD.getMovie(movieKey);

        movie.serialize(result);

        return;
    } 

    private void getMoviePoster(SmartTicketBD smartTicketBD, 
                                DataInputStream call, 
                                DataOutputStream result) throws IOException, 
                                ModelException, ApplicationException {
        String movieKey = call.readUTF();
        byte[] poster = smartTicketBD.getMoviePoster(movieKey);

        result.writeInt(poster.length);
        result.write(poster, 0, poster.length);

        return;
    } 

    private void getMovieShowtimes(SmartTicketBD smartTicketBD, 
                                   DataInputStream call, 
                                   DataOutputStream result) throws IOException, 
                                   ModelException, ApplicationException {
        String theaterKey = call.readUTF();
        String movieKey = call.readUTF();
        int[][] showtimes = smartTicketBD.getMovieShowTimes(theaterKey, 
                movieKey);

        TheaterSchedule.serializeShowTimes(showtimes, result);

        return;
    } 

    private void getSeatingPlan(SmartTicketBD smartTicketBD, 
                                DataInputStream call, 
                                DataOutputStream result) throws IOException, 
                                ModelException, ApplicationException {
        String theaterKey = call.readUTF();
        String movieKey = call.readUTF();
        int[] showtime = new int[3];

        showtime[0] = call.readInt();
        showtime[1] = call.readInt();
        showtime[2] = call.readInt();

        SeatingPlan seatingPlan = smartTicketBD.getSeatingPlan(theaterKey, 
                movieKey, showtime);

        seatingPlan.serialize(result);

        return;
    } 

    private void reserveSeats(SmartTicketBD smartTicketBD, 
                              DataInputStream call, 
                              DataOutputStream result) throws IOException, 
                              ModelException, ApplicationException {
        String theaterKey = call.readUTF();
        String movieKey = call.readUTF();
        int[] showtime = new int[3];

        showtime[0] = call.readInt();
        showtime[1] = call.readInt();
        showtime[2] = call.readInt();

        SeatingPlan.Seat[] seats = new SeatingPlan.Seat[call.readInt()];

        for (int i = 0; i < seats.length; i++) {
            seats[i] = SeatingPlan.Seat.deserialize(call);
        } 

        Reservation reservation = smartTicketBD.reserveSeats(theaterKey, 
                movieKey, showtime, seats);

        reservation.serialize(result);

        return;
    } 

    private void purchaseTickets(SmartTicketBD smartTicketBD, 
                                 DataInputStream call, 
                                 DataOutputStream result) throws IOException, 
                                 ModelException, ApplicationException {
        String reservationId = call.readUTF();

        smartTicketBD.purchaseTickets(reservationId);

        return;
    } 

    private void cancelSeatReservation(SmartTicketBD smartTicketBD, 
                                       DataInputStream call, 
                                       DataOutputStream result) throws IOException, 
                                       ModelException, ApplicationException {
        String reservationId = call.readUTF();

        smartTicketBD.cancelSeatReservation(reservationId);

        return;
    } 

    private void getLocales(SmartTicketBD smartTicketBD, 
                            DataInputStream call, 
                            DataOutputStream result) throws IOException, 
                            ModelException, ApplicationException {
        String[] locales = smartTicketBD.getLocales();

        result.writeInt(locales.length);

        for (int i = 0; i < locales.length; i++) {
            result.writeUTF(locales[i]);
        } 

        return;
    } 

    private void getResourceBundle(SmartTicketBD smartTicketBD, 
                                   DataInputStream call, 
                                   DataOutputStream result) throws IOException, 
                                   ModelException, ApplicationException {
        String baseName = call.readUTF();
        String locale = call.readUTF();
        IndexedResourceBundle bundle = 
            smartTicketBD.getResourceBundle(baseName, locale);

        bundle.serialize(result);

        return;
    } 

    public void initiateSynchronization(SmartTicketBD smartTicketBD, 
                                        DataInputStream call, 
                                        DataOutputStream result) throws IOException, 
                                        ModelException, ApplicationException {
        SyncAnchor syncAnchor = SyncAnchor.deserialize(call);
        long clientTime = call.readLong();

        syncAnchor = smartTicketBD.initiateSynchronization(syncAnchor, 
                clientTime);

        syncAnchor.serialize(result);

        return;
    } 

    public void synchronizeMovieRatings(SmartTicketBD smartTicketBD, 
                                        DataInputStream call, 
                                        DataOutputStream result) throws IOException, 
                                        ModelException, ApplicationException {
        int conflictResolutionStrategyId = call.readInt();
        MovieRating[] changedMovieRatings = new MovieRating[call.readInt()];

        for (int i = 0; i < changedMovieRatings.length; i++) {
            changedMovieRatings[i] = MovieRating.deserialize(call);
        } 

        MovieRating[] updatedMovieRatings = 
            smartTicketBD.synchronizeMovieRatings(changedMovieRatings, 
                                                  conflictResolutionStrategyId);

        if (updatedMovieRatings != null) {
            result.writeInt(updatedMovieRatings.length);

            for (int i = 0; i < updatedMovieRatings.length; i++) {
                updatedMovieRatings[i].serialize(result);
            } 
        } else {
            result.writeInt(0);
        } 

        return;
    } 

    public void commitMovieRatings(SmartTicketBD smartTicketBD, 
                                   DataInputStream call, 
                                   DataOutputStream result) throws IOException, 
                                   ModelException, ApplicationException {
        MovieRating[] resolvedMovieRatings = new MovieRating[call.readInt()];

        for (int i = 0; i < resolvedMovieRatings.length; i++) {
            resolvedMovieRatings[i] = MovieRating.deserialize(call);
        } 

        MovieRating[] updatedMovieRatings = 
            smartTicketBD.commitMovieRatings(resolvedMovieRatings);

        if (updatedMovieRatings != null) {
            result.writeInt(updatedMovieRatings.length);

            for (int i = 0; i < updatedMovieRatings.length; i++) {
                updatedMovieRatings[i].serialize(result);
            } 
        } else {
            result.writeInt(0);
        } 

        return;
    } 

    /**
     * @link dependency
     */
    /* # SmartTicketBD lnkSmartTicketBD; */
}

