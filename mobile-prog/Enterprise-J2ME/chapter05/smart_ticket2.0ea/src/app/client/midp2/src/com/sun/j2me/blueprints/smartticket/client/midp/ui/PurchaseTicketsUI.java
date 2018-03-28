

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
 * $Id: PurchaseTicketsUI.java,v 1.1 2003/04/09 21:33:36 ro89390 Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.AccountInfo;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;

public class PurchaseTicketsUI extends Form implements CommandListener {
    private UIController uiController;
    private AccountInfo accountInfo;
    private Command cancelCommand;
    private Command nextCommand;
    private ChoiceGroup cardList;
    private ChoiceGroup monthList;
    private ChoiceGroup yearList;
    private StringItem signOnMessage;
    private StringItem expDateMessage;
    private TextField cardNumField;
    // private TextField passwordField;
    private Ticker ticker;

    public PurchaseTicketsUI(UIController uiController) {
        super(uiController.getString(UIConstants.SIGN_ON_TITLE));

        this.uiController = uiController;

        createItems();
        append(signOnMessage);
        append(cardList);
        append(expDateMessage);
        append(monthList);
        append(yearList);
        append(cardNumField);
        // append(passwordField);
        setTicker(ticker);

        nextCommand = new Command(uiController.getString(UIConstants.NEXT), 
                                  Command.OK, 5);
        cancelCommand = 
            new Command(uiController.getString(UIConstants.CANCEL), 
                        Command.EXIT, 5);

        addCommand(nextCommand);
        addCommand(cancelCommand);
        setCommandListener(this);
    }

    public void init(AccountInfo accountInfo) {
        this.accountInfo = accountInfo;

        cardNumField.setString(accountInfo.getCreditCardNumber());
        Utils.setExpirationDate(uiController, 
                                accountInfo.getCreditCardExpirationDate(), 
                                monthList, yearList);
    } 

    public void commandAction(Command command, Displayable displayable) {
        if (command == cancelCommand) {
            uiController.mainMenuRequested();
        } else if (command == nextCommand) {
            if (cardNumField.getString().length() == 0) {
                uiController.showErrorAlert(uiController.getString(UIConstants.CREDITCARD_NUM_BAD_LENGTH));

                return;
            } 

            /*
             * String password = passwordField.getString();
             * if (!password.equals(accountInfo.getPassword())) {
             * uiController.showErrorAlert(
             * uiController.getString(UIConstants.PASSWORD_INCORRECT));
             * return;
             * }
             */
            uiController.signOnConfirmed();
        } 
    } 

    private void createItems() {
        cardList = Utils.newCardPopUp(uiController);
        yearList = Utils.newYearPopUp(uiController);
        monthList = Utils.newMonthPopUp(uiController);
        signOnMessage = 
            new StringItem(uiController.getString(UIConstants.SIGN_ON_MESSAGE), 
                           null);
        expDateMessage = 
            new StringItem(uiController.getString(UIConstants.EXPIR_DATE), 
                           null);
        cardNumField = 
            new TextField(uiController.getString(UIConstants.CARD_NUM), null, 
                          19, TextField.NUMERIC);

        /*
         * passwordField = new TextField(
         * uiController.getString(UIConstants.PASSWORD),
         * null, 10, TextField.PASSWORD);
         */
        ticker = 
            new Ticker(uiController.getString(UIConstants.SIGN_ON_TICKER));
    } 

}

