

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
 * $Id: HTTPCommunicationHandler.java,v 1.10 2003/03/27 08:35:28 ro89390 Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.model;

import javax.microedition.io.*;
import java.io.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan.Seat;

public class HTTPCommunicationHandler extends RemoteModelRequestHandler {
    private String serviceURL;
    private boolean offline = true;

    public HTTPCommunicationHandler(RemoteModelRequestHandler nextHandler, 
                                    String serviceURL) {
        super(nextHandler);

        this.serviceURL = serviceURL;

        // serviceURL = preferences.getServiceURL();
        return;
    }

    public boolean isOffline() {
        return offline;
    } 

    public void setOffline(boolean offline) {
        this.offline = offline;

        return;
    } 

    public void login(String userName, String password) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_LOGIN_USER);
            outputStream.writeUTF(userName);
            outputStream.writeUTF(password);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            // Do nothing.
            updateProgress();
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public void createAccount(AccountInfo accountInfo) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_CREATE_ACCOUNT);
            accountInfo.serialize(outputStream);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            // Do nothing.
            updateProgress();
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public void updateAccount(AccountInfo accountInfo) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_UPDATE_ACCOUNT);
            accountInfo.serialize(outputStream);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            // Do nothing.
            updateProgress();
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public Theater[] getTheaters(String zipCode) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_GET_THEATERS);
            outputStream.writeUTF(zipCode);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            Theater[] theaters = new Theater[inputStream.readInt()];

            for (int i = 0; i < theaters.length; i++) {
                theaters[i] = Theater.deserialize(inputStream);
            } 

            updateProgress();

            return theaters;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public TheaterSchedule getTheaterSchedule(String theaterKey) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_GET_THEATER_SCHEDULE);
            outputStream.writeUTF(theaterKey);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            TheaterSchedule theaterSchedule = 
                TheaterSchedule.deserialize(inputStream);

            updateProgress();

            return theaterSchedule;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public Movie getMovie(String movieKey) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_GET_MOVIE);
            outputStream.writeUTF(movieKey);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            Movie movie = Movie.deserialize(inputStream);

            updateProgress();

            return movie;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public byte[] getMoviePoster(String movieKey) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_GET_MOVIE_POSTER);
            outputStream.writeUTF(movieKey);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            byte[] data = new byte[inputStream.readInt()];

            inputStream.readFully(data);
            updateProgress();

            return data;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public int[][] getMovieShowTimes(String theaterKey, String movieKey) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_GET_MOVIE_SHOWTIMES);
            outputStream.writeUTF(theaterKey);
            outputStream.writeUTF(movieKey);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            int[][] showTimes = 
                TheaterSchedule.deserializeShowTimes(inputStream);

            updateProgress();

            return showTimes;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public SeatingPlan getSeatingPlan(String theaterKey, String movieKey, 
                                      int[] showTime) throws ModelException, 
                                      ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_GET_SEATING_PLAN);
            outputStream.writeUTF(theaterKey);
            outputStream.writeUTF(movieKey);

            for (int i = 0; i < showTime.length; i++) {
                outputStream.writeInt(showTime[i]);
            } 

            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            SeatingPlan seatingPlan = SeatingPlan.deserialize(inputStream);

            updateProgress();

            return seatingPlan;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public Reservation reserveSeats(String theaterKey, String movieKey, 
                                    int[] showTime, 
                                    Seat[] seats) throws ModelException, 
                                    ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_RESERVE_SEATS);
            outputStream.writeUTF(theaterKey);
            outputStream.writeUTF(movieKey);

            for (int i = 0; i < showTime.length; i++) {
                outputStream.writeInt(showTime[i]);
            } 

            outputStream.writeInt(seats.length);

            for (int i = 0; i < seats.length; i++) {
                seats[i].serialize(outputStream);
            } 

            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            Reservation reservation = Reservation.deserialize(inputStream);

            updateProgress();

            return reservation;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public void purchaseTickets(String reservationId /* not used for now */) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_PURCHASE_TICKETS);
            outputStream.writeUTF(reservationId);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            // Do nothing.
            updateProgress();
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public void cancelSeatReservation(String reservationId) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_CANCEL_SEAT_RESERVATION);
            outputStream.writeUTF(reservationId);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            // Do nothing.
            updateProgress();
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public SyncAnchor initiateSynchronization(SyncAnchor syncAnchor, long clientTime) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_INITIATE_SYNCHRONIZATION);
            syncAnchor.serialize(outputStream);
            outputStream.writeLong(clientTime);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);
            syncAnchor = SyncAnchor.deserialize(inputStream);

            // updateProgress();
            return syncAnchor;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public MovieRating[] synchronizeMovieRatings(MovieRating[] movieRatings, 
            int conflictResolutionStrategyId) throws ModelException, 
            ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_SYNCHRONIZE_MOVIE_RATINGS);
            outputStream.writeInt(conflictResolutionStrategyId);
            outputStream.writeInt(movieRatings.length);

            for (int i = 0; i < movieRatings.length; i++) {
                movieRatings[i].serialize(outputStream);
            } 

            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            MovieRating[] updatedMovieRatings = 
                new MovieRating[inputStream.readInt()];

            for (int i = 0; i < updatedMovieRatings.length; i++) {
                updatedMovieRatings[i] = MovieRating.deserialize(inputStream);
            } 

            updateProgress();

            return updatedMovieRatings;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public MovieRating[] commitMovieRatings(MovieRating[] movieRatings) 
            throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_COMMIT_MOVIE_RATINGS);
            outputStream.writeInt(movieRatings.length);

            for (int i = 0; i < movieRatings.length; i++) {
                movieRatings[i].serialize(outputStream);
            } 

            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            MovieRating[] updatedMovieRatings = 
                new MovieRating[inputStream.readInt()];

            for (int i = 0; i < updatedMovieRatings.length; i++) {
                updatedMovieRatings[i] = MovieRating.deserialize(inputStream);
            } 

            updateProgress();

            return updatedMovieRatings;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public String[] getLocales() throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_GET_LOCALES);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            String[] locales = new String[inputStream.readInt()];

            for (int i = 0; i < locales.length; i++) {
                locales[i] = inputStream.readUTF();
            } 

            updateProgress();

            return locales;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    public IndexedResourceBundle getResourceBundle(String baseName, 
            String locale) throws ModelException, ApplicationException {
        HttpConnection connection = null;
        DataOutputStream outputStream = null;
        DataInputStream inputStream = null;

        try {
            connection = openConnection();

            updateProgress();

            outputStream = openConnectionOutputStream(connection);

            outputStream.writeByte(MessageConstants.OPERATION_GET_RESOURCE_BUNDLE);
            outputStream.writeUTF(baseName);
            outputStream.writeUTF(locale);
            outputStream.close();
            updateProgress();

            inputStream = openConnectionInputStream(connection);

            IndexedResourceBundle bundle = 
                IndexedResourceBundle.deserialize(inputStream);

            updateProgress();

            return bundle;
        } catch (IOException ioe) {
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } 
        finally {
            closeConnection(connection, outputStream, inputStream);
        } 
    } 

    private HttpConnection openConnection() throws IOException {
        try {
            HttpConnection connection = 
                (HttpConnection) Connector.open(serviceURL);

            connection.setRequestProperty("User-Agent", 
                                          System.getProperty("microedition.profiles"));
            connection.setRequestProperty("Content-Type", 
                                          "application/octet-stream");
            connection.setRequestMethod(HttpConnection.POST);

            offline = false;

            return connection;
        } catch (IOException ioe) {
            offline = true;

            throw ioe;
        } 
    } 

    private DataOutputStream openConnectionOutputStream(HttpConnection connection) 
            throws IOException {
        try {
            return connection.openDataOutputStream();
        } catch (IOException ioe) {
            offline = true;

            throw ioe;
        } 
    } 

    private DataInputStream openConnectionInputStream(HttpConnection connection) 
            throws IOException, ModelException, ApplicationException {
        try {
            int responseCode = connection.getResponseCode();

            if (responseCode == HttpConnection.HTTP_OK 
                    || responseCode == HttpConnection.HTTP_CREATED) {
                if (responseCode == HttpConnection.HTTP_CREATED) {
                    serviceURL = connection.getHeaderField("Location");
                } 

                DataInputStream inputStream = 
                    connection.openDataInputStream();
                int returnCode = inputStream.readInt();

                switch (returnCode) {

                case MessageConstants.ERROR_NONE: {
                    return inputStream;
                } 

                case MessageConstants.ERROR_MODEL_EXCEPTION: {
                    try {
                        throw ModelException.deserialize(inputStream);
                    } 
                    finally {
                        try {
                            inputStream.close();
                        } catch (IOException ioe) {}    // Ignored
                    } 
                } 

                default: {
                    throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
                } 
                }
            } 

            /*
             * else if (responseCode == HttpConnection.HTTP_INTERNAL_ERROR) {
             * throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
             * }
             */
            throw new ApplicationException(ErrorMessageCodes.ERROR_CANNOT_CONNECT);
        } catch (IOException ioe) {
            offline = true;

            throw ioe;
        } 
    } 

    void closeConnection(HttpConnection connection, 
                         DataOutputStream outputStream, 
                         DataInputStream inputStream) {
        if (outputStream != null) {
            try {
                outputStream.close();
            } catch (IOException ioe) {}    // Ignored
        } 

        if (inputStream != null) {
            try {
                inputStream.close();
            } catch (IOException ioe) {}    // Ignored
        } 

        if (connection != null) {
            try {
                connection.close();
            } catch (IOException ioe) {}    // Ignored
        } 

        return;
    } 

}

