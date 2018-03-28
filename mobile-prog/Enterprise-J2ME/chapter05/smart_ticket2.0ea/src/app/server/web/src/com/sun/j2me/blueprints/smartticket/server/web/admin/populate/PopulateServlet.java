

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
 * $Id: PopulateServlet.java,v 1.4 2003/03/24 22:43:11 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.web.admin.populate;

import javax.naming.*;
import javax.servlet.*;
import javax.servlet.http.*;
import javax.sql.*;
import javax.xml.parsers.*;
import java.io.*;
import java.net.*;
import java.sql.*;
import java.util.*;
import org.xml.sax.*;
import org.xml.sax.helpers.*;

/**
 * @servletName PopulateServlet
 * @webURLPattern /populate
 */
public class PopulateServlet extends HttpServlet {
    private static final String POPULATE_DATA_PATH_PARAM = "PopulateData";
    private static final String DEFAULT_POPULATE_DATA_PATH = 
        "/populate/Populate-UTF8.xml";
    private static final String SUCCESS_PAGE_URL_PARAM = "success_page";
    private static final String ERROR_PAGE_URL_PARAM = "error_page";
    private static final String FORCEFULLY_MODE_PARAM = "forcefully";
    private static final String REFERER_HEADER = "Referer";
    private String populateDataPath;

    /**
     * @link dependency
     */
    /* # TheaterPopulator lnkTheaterPopulator; */

    /**
     * @link dependency
     */
    /* # MoviePopulator lnkMoviePopulator; */

    /**
     * @link dependency
     */
    /* # AccountPopulator lnkAccountPopulator; */
    public void init(ServletConfig config) throws ServletException {
        super.init(config);

        try {
            SAXParserFactory parserFactory = SAXParserFactory.newInstance();

            parserFactory.setValidating(true);
            parserFactory.setNamespaceAware(true);

            populateDataPath = 
                config.getInitParameter(POPULATE_DATA_PATH_PARAM);

            if (populateDataPath == null) {
                populateDataPath = DEFAULT_POPULATE_DATA_PATH;
            } 
        } catch (Exception exception) {
            throw new ServletException(exception);
        } 

        return;
    } 

    public void doGet(HttpServletRequest request, 
                      HttpServletResponse response) throws IOException, 
                      ServletException {
        doPost(request, response);

        return;
    } 

    public void doPost(HttpServletRequest request, 
                       HttpServletResponse response) throws IOException, 
                       ServletException {
        String successPageURL = request.getParameter(SUCCESS_PAGE_URL_PARAM);
        String errorPageURL = request.getParameter(ERROR_PAGE_URL_PARAM);
        String forcefully = request.getParameter(FORCEFULLY_MODE_PARAM);
        String refererURL = request.getHeader(REFERER_HEADER);

        // kill the Servlet Session because all EJB Refs are no longer valid
        request.getSession().invalidate();

        try {
            populate(forcefully != null 
                     && Boolean.valueOf(forcefully).booleanValue());
        } catch (PopulateException exception) {
            System.err.println(exception.getRootCause().getMessage());

            if (errorPageURL == null) {
                throw new ServletException("Populate exception occured :" 
                                           + exception.getMessage(), exception.getRootCause());
            } else {
                redirect(request, response, errorPageURL);
            } 
        } 

        if (successPageURL != null) {
            redirect(request, response, successPageURL);
        } else if (refererURL != null) {
            redirect(request, response, refererURL);
        } 

        return;
    } 

    private boolean populate(boolean forcefully) throws PopulateException {
        XMLReader reader = null;

        try {
            SAXParserFactory parserFactory = SAXParserFactory.newInstance();

            parserFactory.setValidating(true);
            parserFactory.setNamespaceAware(true);

            reader = parserFactory.newSAXParser().getXMLReader();
        } catch (Exception exception) {
            throw new PopulateException(exception);
        } 

        boolean alreadyPopulated = false;
        MoviePopulator moviePopulator = new MoviePopulator();
        TheaterPopulator theaterPopulator = new TheaterPopulator();
        AccountPopulator accountPopulator = new AccountPopulator();

        if (!forcefully) {
            try {
                alreadyPopulated = moviePopulator.check() 
                                   && theaterPopulator.check() 
                                   && accountPopulator.check();
            } catch (PopulateException exception) {}
        } 

        if (forcefully ||!alreadyPopulated) {
            try {
                moviePopulator.clear();
                theaterPopulator.clear();
                accountPopulator.clear();
                moviePopulator.setup(theaterPopulator.setup(accountPopulator.setup(reader))).parse(new InputSource(getServletConfig().getServletContext().getResourceAsStream(populateDataPath)));
            } catch (Exception exception) {
                throw new PopulateException(exception);
            } 
        } 

        return forcefully ||!alreadyPopulated;
    } 

    private void redirect(HttpServletRequest request, 
                          HttpServletResponse response, 
                          String path) throws IOException {
        String url;

        if (path.startsWith("//")) {
            url = 
                new URL(new URL(HttpUtils.getRequestURL(request).toString()), 
                        path.substring(1)).toString();
        } else {
            url = request.getContextPath() 
                  + (path.startsWith("/") ? path : "/" + path);
        } 

        response.sendRedirect(url);

        return;
    } 

    private String getResource(String path) throws IOException {
        String url;

        try {
            url = new URL(path).toString();
        } catch (MalformedURLException exception) {
            URL u = getServletContext().getResource(path);

            url = u != null ? u.toString() : path;
        } 

        return url;
    } 

    private class ParsingDoneException extends SAXException {
        ParsingDoneException() {
            super("");
        }

    }

}

