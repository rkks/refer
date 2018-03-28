

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
 * $Id: RateMyMoviesUI.java,v 1.3 2003/04/11 22:52:07 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.MovieRating;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.Movie;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class RateMyMoviesUI extends Form implements CommandListener, 
        ItemStateListener {
    private Command deleteCommand, syncCommand, saveCommand, cancelCommand;
    private Gauge[] movieRatesUI;    // should use a custom item
    private int[] movieRatesNum;
    private MovieRating[] movieRatings;
    private boolean[] modifiedMovieRatings;
    private StringItem[] movieNames;
    private StringItem[] lastViewDates;
    private UIController uiController;

    public RateMyMoviesUI(UIController uiController) {
        super(uiController.getString(UIConstants.RATE_MY_MOVIES_TITLE));

        this.uiController = uiController;

        /*
         * deleteCommand
         * = new Command(uiController.getString(UIConstants.DELETE),
         * Command.OK, 2);
         */
        syncCommand = new Command(uiController.getString(UIConstants.SYNC), 
                                  Command.OK, 2);

        // addCommand(deleteCommand);
        addCommand(syncCommand);

        saveCommand = new Command(uiController.getString(UIConstants.SAVE), 
                                  Command.OK, 1);

        addCommand(saveCommand);

        cancelCommand = 
            new Command(uiController.getString(UIConstants.CANCEL), 
                        Command.CANCEL, 1);

        addCommand(cancelCommand);
        setCommandListener(this);
        setItemStateListener(this);
    }

    /**
     * Initialize the seating canvas variables.
     * @exception IOException  when some type of network error occurs.
     */
    public void init(MovieRating[] movieRatings) throws ApplicationException {
        this.movieRatings = movieRatings;

        int numOfRatings = movieRatings.length;

        deleteAll();

        // create items
        movieRatesUI = new Gauge[numOfRatings];
        movieRatesNum = new int[numOfRatings];
        modifiedMovieRatings = new boolean[numOfRatings];
        movieNames = new StringItem[numOfRatings];
        lastViewDates = new StringItem[numOfRatings];

        for (int i = 0; i < numOfRatings; i++) {
            movieRatesNum[i] = movieRatings[i].getStarNumber();
            movieNames[i] = 
                new StringItem(null, movieRatings[i].getMovie().getTitle());
            // need to parse the date value

            /*
             * lastViewDates[i] =  new StringItem(null,
             * movieRatings[i].getLastViewingDate()+"");
             */

            /*
             * if (movieRatesNum[i] == MovieRating.RATING_UNRATED) {
             * movieRatesUI[i].setLabel(uiController.getString(UIConstants.UNRATED));
             * }
             */
            movieRatesUI[i] = new Gauge(null, true, 
                                        MovieRating.RATING_FIVE_STARS, 
                                        movieRatesNum[i]);
            modifiedMovieRatings[i] = false;

            movieNames[i].setLayout(Item.LAYOUT_CENTER);
            // lastViewDates[i].setLayout(Item.LAYOUT_CENTER);
            movieRatesUI[i].setLayout(Item.LAYOUT_CENTER);
            append(movieRatesUI[i]);
            append(movieNames[i]);
            // append(lastViewDates[i]);
        } 
    } 

    public void commandAction(Command command, Displayable displayable) {
        if (command == syncCommand) {
            uiController.rateMyMoviesSyncRequested();
        } else if (command == saveCommand) {
            MovieRating[] tmp = new MovieRating[movieRatings.length];
            int j = 0;

            for (int i = 0; i < movieRatings.length; i++) {
                if (modifiedMovieRatings[i]) {
                    tmp[j++] = movieRatings[i];
                    modifiedMovieRatings[i] = false;
                } 
            } 

            if (j > 0) {
                System.out.println("j:" + j);

                MovieRating[] modifiedList = new MovieRating[j];

                System.arraycopy(tmp, 0, modifiedList, 0, j);
                uiController.rateMyMoviesSaveRequested(modifiedList);
            } 
        } else if (command == cancelCommand) {
            uiController.mainMenuRequested();
        } else {
            uiController.commandAction(command, displayable);
        } 
    } 

    public void itemStateChanged(Item item) {
        for (int i = 0; i < movieRatesUI.length; i++) {
            if (item == movieRatesUI[i]) {
                movieRatings[i].setStarNumber(movieRatesUI[i].getValue());

                modifiedMovieRatings[i] = true;
            } 
        } 
    } 

}

