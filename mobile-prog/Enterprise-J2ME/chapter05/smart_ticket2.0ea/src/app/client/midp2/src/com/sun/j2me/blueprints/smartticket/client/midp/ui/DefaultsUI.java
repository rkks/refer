

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
 * $Id: DefaultsUI.java,v 1.1 2003/04/05 00:41:51 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.client.midp.model.Preferences;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;

public class DefaultsUI extends Form implements CommandListener {
    private UIController uiController;
    private Preferences preferences;
    private Command saveCommand;
    private Command cancelCommand;
    private TextField firstZipField;
    private TextField secondZipField;
    private TextField numOfTicketsField;
    private ChoiceGroup favoriteDayList;
    private ChoiceGroup favoriteSeatList;
    private ChoiceGroup updateMovieList;
    private ChoiceGroup listMovieList;
    private StringItem suggestedOrderMsg;

    public DefaultsUI(UIController uiController) {
        super(uiController.getString(UIConstants.DEFAULTS_TITLE));

        this.uiController = uiController;

        createItems();
        append(firstZipField);
        append(secondZipField);
        append(numOfTicketsField);
        append(favoriteDayList);
        append(favoriteSeatList);
        append(updateMovieList);
        append(listMovieList);
        append(suggestedOrderMsg);

        saveCommand = new Command(uiController.getString(UIConstants.SAVE), 
                                  Command.OK, 5);
        cancelCommand = 
            new Command(uiController.getString(UIConstants.CANCEL), 
                        Command.CANCEL, 5);

        addCommand(saveCommand);
        addCommand(cancelCommand);
        setCommandListener(this);
    }

    public void init(Preferences preferences) {
        this.preferences = preferences;

        firstZipField.setString(preferences.getDefaultZipCode());
        secondZipField.setString(preferences.getSecondZipCode());
        numOfTicketsField.setString(String.valueOf(preferences.getDefaultTicketNumber()));
        Utils.setChoiceGroupEntry(uiController, preferences.getFavoriteDay(), 
                                  favoriteDayList);
        Utils.setChoiceGroupEntry(uiController, 
                                  preferences.getFavoriteSeat(), 
                                  favoriteSeatList);
        Utils.setChoiceGroupEntry(uiController, 
                                  preferences.getUpdateMovieSchedule(), 
                                  updateMovieList);
        Utils.setChoiceGroupEntry(uiController, preferences.getListMovie(), 
                                  listMovieList);
    } 

    public void commandAction(Command command, Displayable displayable) {
        if (command == cancelCommand) {
            uiController.preferencesExited();
        } else if (command == saveCommand) {
            String firstZip = null;
            int numTickets = -1;

            firstZip = firstZipField.getString();

            if (firstZip == null 
                    || firstZip.length() != UIConstants.ZIP_LENGTH) {
                uiController.showErrorAlert(uiController.getString(UIConstants.ZIP_CODE_BAD_LENGTH));

                return;
            } 

            String secondZip = secondZipField.getString();

            // it's not necessary to have 2nd zip code, but it's required to
            // be 5 characters long
            if (secondZip != null && secondZip.length() != 0 
                    && secondZip.length() != UIConstants.ZIP_LENGTH) {
                uiController.showErrorAlert(uiController.getString(UIConstants.ZIP_CODE_BAD_LENGTH));

                return;
            } 

            numTickets = Integer.parseInt(numOfTicketsField.getString());

            if (numTickets < 1) {
                uiController.showErrorAlert(uiController.getString(UIConstants.NUM_OF_TICKET_ERR));

                return;
            } 

            preferences.setDefaultZipCode(firstZip);

            if (secondZip != null) {
                preferences.setSecondZipCode(secondZip);
            } 

            preferences.setDefaultTicketNumber(numTickets);
            preferences.setFavoriteDay(getFavoriteDayListSelection());
            preferences.setFavoriteSeat(getFavoriteSeatListSelection());
            preferences.setUpdateMovieSchedule(getUpdateMovieListSelection());
            preferences.setListMovie(getListMovieListSelection());
            uiController.preferencesSaved(preferences);
        } 
    } 

    private void createItems() {
        firstZipField = 
            new TextField(uiController.getString(UIConstants.FIRST_ZIP), "", 
                          10, TextField.ANY);
        secondZipField = 
            new TextField(uiController.getString(UIConstants.SECOND_ZIP), "", 
                          10, TextField.ANY);
        numOfTicketsField = 
            new TextField(uiController.getString(UIConstants.DEFAULT_NUM_OF_TICKETS), 
                          "", 3, TextField.NUMERIC);
        suggestedOrderMsg = 
            new StringItem(uiController.getString(UIConstants.SUGGESTED_ORDER_MSG), 
                           null);
        updateMovieList = 
            new ChoiceGroup(uiController.getString(UIConstants.UPDATE_MOVIE_SCHEDULE), 
                            Choice.POPUP);
        listMovieList = 
            new ChoiceGroup(uiController.getString(UIConstants.LIST_MOVIE), 
                            Choice.POPUP);
        // initialize those pop up lists
        favoriteSeatList = 
            Utils.newPopUp(uiController, 
                           uiController.getString(UIConstants.PREFERRED_SEAT), 
                           UIConstants.PREF_SEAT_START, 
                           UIConstants.PREF_SEAT_END);
        favoriteDayList = 
            Utils.newPopUp(uiController, 
                           uiController.getString(UIConstants.FAVORITE_DAY), 
                           UIConstants.PREF_DATE_START, 
                           UIConstants.PREF_DATE_END);

        updateMovieList.append(uiController.getString(UIConstants.AUTOMATICALLY), 
                               null);
        updateMovieList.append(uiController.getString(UIConstants.WHEN_I_SIGN_IN), 
                               null);
        updateMovieList.append(uiController.getString(UIConstants.MANUALLY), 
                               null);
        listMovieList.append(uiController.getString(UIConstants.SUGGESTED_ORDER), 
                             null);
        listMovieList.append(uiController.getString(UIConstants.ALPHAETICALLY), 
                             null);
    } 

    private int getFavoriteDayListSelection() {
        ChoiceGroup choiceGroup = favoriteDayList;
        String str = choiceGroup.getString(choiceGroup.getSelectedIndex());

        if (str.equals(uiController.getString(UIConstants.PREF_SUN))) {
            return UIConstants.PREF_SUN;
        } else if (str.equals(uiController.getString(UIConstants.PREF_SUN))) {
            return UIConstants.PREF_SUN;
        } else if (str.equals(uiController.getString(UIConstants.PREF_MON))) {
            return UIConstants.PREF_MON;
        } else if (str.equals(uiController.getString(UIConstants.PREF_TUE))) {
            return UIConstants.PREF_TUE;
        } else if (str.equals(uiController.getString(UIConstants.PREF_WED))) {
            return UIConstants.PREF_WED;
        } else if (str.equals(uiController.getString(UIConstants.PREF_THU))) {
            return UIConstants.PREF_THU;
        } else if (str.equals(uiController.getString(UIConstants.PREF_FRI))) {
            return UIConstants.PREF_FRI;
        } else if (str.equals(uiController.getString(UIConstants.PREF_SAT))) {
            return UIConstants.PREF_SAT;
        } 

        return UIConstants.PREF_SUN;    // default
    } 

    private int getFavoriteSeatListSelection() {
        ChoiceGroup choiceGroup = favoriteSeatList;
        String str = choiceGroup.getString(choiceGroup.getSelectedIndex());

        if (str.equals(uiController.getString(UIConstants.PREF_LEFT_F))) {
            return UIConstants.PREF_LEFT_F;
        } else if (str.equals(uiController.getString(UIConstants.PREF_LEFT_B))) {
            return UIConstants.PREF_LEFT_B;
        } else if (str.equals(uiController.getString(UIConstants.PREF_CENTER_F))) {
            return UIConstants.PREF_CENTER_F;
        } else if (str.equals(uiController.getString(UIConstants.PREF_CENTER_M))) {
            return UIConstants.PREF_CENTER_M;
        } else if (str.equals(uiController.getString(UIConstants.PREF_CENTER_B))) {
            return UIConstants.PREF_CENTER_B;
        } else if (str.equals(uiController.getString(UIConstants.PREF_RIGHT_F))) {
            return UIConstants.PREF_RIGHT_F;
        } else if (str.equals(uiController.getString(UIConstants.PREF_RIGHT_B))) {
            return UIConstants.PREF_RIGHT_B;
        } 

        return UIConstants.PREF_CENTER_M;    // default
    } 

    private int getUpdateMovieListSelection() {
        ChoiceGroup choiceGroup = updateMovieList;
        String str = choiceGroup.getString(choiceGroup.getSelectedIndex());

        if (str.equals(uiController.getString(UIConstants.AUTOMATICALLY))) {
            return UIConstants.AUTOMATICALLY;
        } else if (str.equals(uiController.getString(UIConstants.WHEN_I_SIGN_IN))) {
            return UIConstants.WHEN_I_SIGN_IN;
        } else if (str.equals(uiController.getString(UIConstants.MANUALLY))) {
            return UIConstants.MANUALLY;
        } 

        return UIConstants.AUTOMATICALLY;    // default
    } 

    private int getListMovieListSelection() {
        ChoiceGroup choiceGroup = listMovieList;
        String str = choiceGroup.getString(choiceGroup.getSelectedIndex());

        if (str.equals(uiController.getString(UIConstants.SUGGESTED_ORDER))) {
            return UIConstants.SUGGESTED_ORDER;
        } else if (str.equals(uiController.getString(UIConstants.ALPHAETICALLY))) {
            return UIConstants.ALPHAETICALLY;
        } 

        return UIConstants.SUGGESTED_ORDER;    // default
    } 

}

