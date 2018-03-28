

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
 * $Id: TheaterSchedule.java,v 1.3 2003/03/20 01:46:53 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.shared.midp.model;

import java.io.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

/*
 * Usage:
 * TheaterSchedule theaterSchedule = new TheaterSchedule(primaryKey, expirationDate);
 * MovieSchedule movieSchedules = new MovieSchedule[] {
 * theaterSchedule.new MovieSchedule(movie1, showTimes1),
 * theaterSchedule.new MovieSchedule(movie2, showTimes2)
 * };
 * theaterSchedule.setMovieSchedules(movieSchedules);
 */
public class TheaterSchedule {
    private String primaryKey;    // Same as the theater primary key
    private long expirationDate;
    private MovieSchedule[] movieSchedules;

    public class MovieSchedule {
        private Movie movie;
        private int[][] showTimes = null;

        public MovieSchedule(Movie movie, int[][] showTimes) {
            this.movie = movie;
            this.showTimes = showTimes != null ? expandShowTimes(showTimes) 
                             : new int[0][0];

            return;
        }

        private MovieSchedule() {}

        public Movie getMovie() {
            return movie;
        } 

        public int[][] getShowTimes() throws ApplicationException {
            if (showTimes.length == 0) {
                if (ModelObjectLoader.getInstance() != null) {
                    try {
                        showTimes = 
                            expandShowTimes(ModelObjectLoader.getInstance().getMovieShowTimes(primaryKey, 
                                movie.getPrimaryKey()));
                    } catch (ModelException me) {
                        throw new ApplicationException();
                    } 
                } 
            } 

            return showTimes;
        } 

        public SeatingPlan getSeatingPlan(int[] showTime) 
                throws ApplicationException {
            try {
                return ModelObjectLoader.getInstance().getSeatingPlan(primaryKey, 
                        movie.getPrimaryKey(), 
                        showTime);    // SeatingPlan are not cached
            } catch (ModelException me) {
                throw new ApplicationException();
            } 
        } 

        private int[][] expandShowTimes(int[][] showTimePatterns) {
            int count = 0;

            for (int i = 0; i < showTimePatterns.length; i++) {
                count += showTimePatterns[i][0] == -1 ? 7 : 1;
            } 

            int[][] showTimes = new int[count][3];
            int j = 0;

            for (int i = 0; i < showTimePatterns.length; i++) {
                if (showTimePatterns[i][0] == -1) {
                    showTimes[j++] = new int[] {
                        Calendar.SUNDAY, showTimePatterns[i][1], 
                        showTimePatterns[i][2]
                    };
                    showTimes[j++] = new int[] {
                        Calendar.MONDAY, showTimePatterns[i][1], 
                        showTimePatterns[i][2]
                    };
                    showTimes[j++] = new int[] {
                        Calendar.TUESDAY, showTimePatterns[i][1], 
                        showTimePatterns[i][2]
                    };
                    showTimes[j++] = new int[] {
                        Calendar.WEDNESDAY, showTimePatterns[i][1], 
                        showTimePatterns[i][2]
                    };
                    showTimes[j++] = new int[] {
                        Calendar.THURSDAY, showTimePatterns[i][1], 
                        showTimePatterns[i][2]
                    };
                    showTimes[j++] = new int[] {
                        Calendar.FRIDAY, showTimePatterns[i][1], 
                        showTimePatterns[i][2]
                    };
                    showTimes[j++] = new int[] {
                        Calendar.SATURDAY, showTimePatterns[i][1], 
                        showTimePatterns[i][2]
                    };
                } else {
                    showTimes[j++] = showTimePatterns[i];
                } 
            } 

            return showTimes;
        } 

    }

    public TheaterSchedule(String primaryKey, long expirationDate) {
        this.primaryKey = primaryKey;
        this.expirationDate = expirationDate;

        return;
    }

    private TheaterSchedule() {}

    public String getPrimaryKey() {
        return primaryKey;
    } 

    public long getExpirationDate() {
        return expirationDate;
    } 

    public MovieSchedule[] getMovieSchedules() {
        return movieSchedules;
    } 

    public MovieSchedule getMovieSchedule(Movie movie) {
        for (int i = 0; i < movieSchedules.length; i++) {
            if (movie.equals(movieSchedules[i].movie)) {
                return movieSchedules[i];
            } 
        } 

        return null;
    } 

    public void setMovieSchedules(MovieSchedule[] movieSchedules) {
        this.movieSchedules = movieSchedules;

        return;
    } 

    public void serialize(DataOutputStream dataStream, 
                          boolean complete) throws ApplicationException {
        try {
            dataStream.writeUTF(primaryKey);
            dataStream.writeLong(expirationDate);
            dataStream.writeInt(movieSchedules.length);
            dataStream.writeBoolean(complete);

            for (int i = 0; i < movieSchedules.length; i++) {
                if (complete) {
                    movieSchedules[i].movie.serialize(dataStream);
                } else {
                    dataStream.writeUTF(movieSchedules[i].movie.getPrimaryKey());
                } 

                serializeShowTimes(movieSchedules[i].getShowTimes(), 
                                   dataStream);
            } 

            return;
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public static TheaterSchedule deserialize(DataInputStream dataStream) 
            throws ApplicationException {
        try {
            TheaterSchedule theaterSchedule = new TheaterSchedule();

            theaterSchedule.primaryKey = dataStream.readUTF();
            theaterSchedule.expirationDate = dataStream.readLong();
            theaterSchedule.movieSchedules = 
                new MovieSchedule[dataStream.readInt()];

            boolean complete = dataStream.readBoolean();

            for (int i = 0; i < theaterSchedule.movieSchedules.length; i++) {
                theaterSchedule.movieSchedules[i] = 
                    theaterSchedule.new MovieSchedule();

                if (complete) {
                    theaterSchedule.movieSchedules[i].movie = 
                        Movie.deserialize(dataStream);
                } else {
                    try {
                        theaterSchedule.movieSchedules[i].movie = 
                            ModelObjectLoader.getInstance().getMovie(dataStream.readUTF());
                    } catch (ModelException me) {
                        throw new ApplicationException();
                    } 
                } 

                theaterSchedule.movieSchedules[i].showTimes = 
                    deserializeShowTimes(dataStream);
            } 

            return theaterSchedule;
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public static void serializeShowTimes(int[][] showTimes, DataOutputStream dataStream) 
            throws ApplicationException {
        try {
            dataStream.writeInt(showTimes.length);

            for (int j = 0; j < showTimes.length; j++) {
                for (int k = 0; k < showTimes[j].length; k++) {
                    dataStream.writeInt(showTimes[j][k]);
                } 
            } 

            return;
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public static int[][] deserializeShowTimes(DataInputStream dataStream) 
            throws ApplicationException {
        try {
            int[][] showTimes = new int[dataStream.readInt()][3];

            for (int j = 0; j < showTimes.length; j++) {
                for (int k = 0; k < showTimes[j].length; k++) {
                    showTimes[j][k] = dataStream.readInt();
                } 
            } 

            return showTimes;
        } catch (IOException ioe) {
            throw new ApplicationException(ioe);
        } 
    } 

    public static boolean matchShowtime(int[] time, int[] showtimes) {
        return (showtimes[0] == -1 || time[0] == showtimes[0]) 
               && time[1] == showtimes[1] && time[2] == showtimes[2];
    } 

}

