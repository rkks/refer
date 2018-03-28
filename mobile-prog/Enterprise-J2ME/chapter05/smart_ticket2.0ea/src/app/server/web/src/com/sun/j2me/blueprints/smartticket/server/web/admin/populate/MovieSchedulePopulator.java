

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
 * $Id: MovieSchedulePopulator.java,v 1.2 2003/03/24 22:43:11 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.web.admin.populate;

import javax.ejb.*;
import javax.naming.*;
import javax.xml.parsers.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocalHome;
import com.sun.j2me.blueprints.smartticket.server.ejb.schedule.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.show.Seating;
import com.sun.j2me.blueprints.smartticket.server.ejb.theater.TheaterLocal;
import org.xml.sax.XMLFilter;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.*;

public class MovieSchedulePopulator {
    private static final String EJB_REF_MOVIE_SCHEDULE = "ejb/MovieSchedule";
    private static final String EJB_REF_MOVIE = "ejb/Movie";
    private static final String XML_MOVIE_SCHEDULE = "MovieSchedule";
    private static final String XML_MOVIE = "MovieSchedule/@movie";
    private static final String XML_SHOW_TIME_PATTERN = "ShowTimePattern";
    private static final String XML_SEATING_ROW = "SeatingRow";
    private String rootTag;
    private MovieScheduleLocalHome movieScheduleHome = null;
    private MovieLocalHome movieHome = null;
    private List movieSchedules = new ArrayList();
    private TheaterLocal theater;

    public MovieSchedulePopulator(String rootTag) {
        this.rootTag = rootTag;

        return;
    }

    public XMLFilter setup(XMLReader reader) throws PopulateException {
        return new XMLDBHandler(reader, rootTag, XML_MOVIE_SCHEDULE) {
            public void update() throws PopulateException {}

            public void create() throws PopulateException {
                List showTimePatterns = new ArrayList();
                String value;
                StringBuffer key = new StringBuffer(XML_SHOW_TIME_PATTERN);

                for (int i = 0; 
                        (value = getValue(key.append('[').append(i).append(']').toString())) != null; 
                        i++) {
                    showTimePatterns.add(new ShowTimePattern(value));
                    key.delete(XML_SHOW_TIME_PATTERN.length(), key.length());
                } 

                ArrayList seatingRows = new ArrayList();

                key = new StringBuffer(XML_SEATING_ROW);

                for (int i = 0; 
                        (value = getValue(key.append('[').append(i).append(']').toString())) != null; 
                        i++) {
                    seatingRows.add(value);
                    key.delete(XML_SEATING_ROW.length(), key.length());
                } 

                Seating seating = 
                    new Seating(seatingRows.size(), 
                                seatingRows.size() > 0 
                                ? ((String) seatingRows.get(0)).length() : 0);

                for (int i = 0; i < seatingRows.size(); i++) {
                    seating.setRow(i, (String) seatingRows.get(i));
                } 

                movieSchedules.add(createMovieSchedule(getValue(XML_MOVIE), 
                                                       (ShowTimePattern[]) showTimePatterns.toArray(new ShowTimePattern[0]), 
                                                       seating));

                return;
            } 

        };
    } 

    public boolean check() throws PopulateException {
        return true;
    } 

    private MovieScheduleLocal createMovieSchedule(String movieId, 
            ShowTimePattern[] showTimePatterns, 
            Seating seating) throws PopulateException {
        try {
            if (movieScheduleHome == null) {
                Context context = 
                    (Context) new InitialContext().lookup("java:comp/env");

                movieScheduleHome = 
                    (MovieScheduleLocalHome) context.lookup(EJB_REF_MOVIE_SCHEDULE);
            } 

            if (movieHome == null) {
                Context context = 
                    (Context) new InitialContext().lookup("java:comp/env");

                movieHome = (MovieLocalHome) context.lookup(EJB_REF_MOVIE);
            } 

            MovieScheduleLocal movieSchedule = 
                movieScheduleHome.create(theater.getId(), 
                                         movieHome.findByPrimaryKey(movieId));

            movieSchedule.setShowTimePatterns(showTimePatterns);
            movieSchedule.setSeating(seating);

            return movieSchedule;
        } catch (Exception exception) {
            throw new PopulateException("Could not create: " 
                                        + exception.getMessage(), exception);
        } 
    } 

    public Collection getMovieSchedules() {
        Collection schedules = new ArrayList(movieSchedules);

        movieSchedules.clear();

        return schedules;
    } 

    public void setTheater(TheaterLocal theater) {
        this.theater = theater;

        return;
    } 

}

