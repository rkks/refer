

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
 * $Id: MoviePopulator.java,v 1.4 2003/03/24 22:43:11 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.web.admin.populate;

import javax.ejb.*;
import javax.naming.*;
import javax.xml.parsers.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocalHome;
import org.xml.sax.XMLFilter;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.*;

public class MoviePopulator {
    private static final String EJB_REF_MOVIE = "ejb/Movie";
    private static final String XML_MOVIES = "Movies";
    private static final String XML_MOVIE = "Movie";
    private static final String XML_ID = "Movie/@id";
    private static final String XML_TITLE = "Title";
    private static final String XML_SUMMARY = "Summary";
    private static final String XML_RATING = "Rating";
    private static final String XML_POSTER_URL = "PosterURL";
    private String rootTag;
    private MovieLocalHome movieHome = null;
    private MovieLocal movie;

    public MoviePopulator() throws PopulateException {
        this(XML_MOVIES);

        return;
    }

    public MoviePopulator(String rootTag) {
        this.rootTag = rootTag;

        return;
    }

    public XMLFilter setup(XMLReader reader) throws PopulateException {
        return new XMLDBHandler(reader, rootTag, XML_MOVIE) {
            public void update() throws PopulateException {}

            public void create() throws PopulateException {
                movie = createMovie(getValue(XML_ID), getValue(XML_TITLE), 
                                    getValue(XML_SUMMARY), 
                                    getValue(XML_RATING), 
                                    getValue(XML_POSTER_URL));

                return;
            } 

        };
    } 

    public boolean check() throws PopulateException {
        try {
            if (movieHome == null) {
                Context context = 
                    (Context) new InitialContext().lookup("java:comp/env");

                movieHome = (MovieLocalHome) context.lookup(EJB_REF_MOVIE);
            } 

            Collection movies = movieHome.findAllMovies();

            if ((movies == null) || (movies.size() == 0)) {
                return false;
            } 
        } catch (Exception exception) {
            return false;
        } 

        return true;
    } 

    public void clear() throws PopulateException {
        try {
            if (movieHome == null) {
                Context context = 
                    (Context) new InitialContext().lookup("java:comp/env");

                movieHome = (MovieLocalHome) context.lookup(EJB_REF_MOVIE);
            } 

            Collection movies = movieHome.findAllMovies();

            for (Iterator i = movies.iterator(); i.hasNext(); ) {
                ((MovieLocal) i.next()).remove();
            } 
        } catch (Exception exception) {
            throw new PopulateException("Could not clear: " 
                                        + exception.getMessage(), exception);
        } 

        return;
    } 

    private MovieLocal createMovie(String id, String title, String summary, 
                                   String rating, 
                                   String posterURL) throws PopulateException {
        try {
            if (movieHome == null) {
                Context context = 
                    (Context) new InitialContext().lookup("java:comp/env");

                movieHome = (MovieLocalHome) context.lookup(EJB_REF_MOVIE);
            } 

            MovieLocal movie = movieHome.create(id);

            movie.setTitle(title);
            movie.setSummary(summary);
            movie.setRating(rating);
            movie.setPosterURL(posterURL);

            return movie;
        } catch (Exception exception) {
            throw new PopulateException("Could not create: " 
                                        + exception.getMessage(), exception);
        } 
    } 

    public MovieLocal getMovie() {
        return movie;
    } 

}

