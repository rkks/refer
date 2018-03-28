

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
 * $Id: BillingInfoUI.java,v 1.1 2003/04/05 00:41:51 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.AccountInfo;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;

public class BillingInfoUI extends Form implements CommandListener, 
        ItemCommandListener {
    private UIController uiController;
    private AccountInfo accountInfo;
    private Command exitCommand;
    private Command doneCommand;
    private ChoiceGroup cardList;
    private ChoiceGroup monthList;
    private ChoiceGroup yearList;
    private ChoiceGroup fillOutLater;
    private StringItem done;
    private StringItem expDateMessage;
    private TextField creditCardNumField;

    public BillingInfoUI(UIController uiController) {
        super(uiController.getString(UIConstants.BILLING_INFORMATION));

        this.uiController = uiController;

        createItems();
        append(cardList);
        append(expDateMessage);
        append(monthList);
        append(yearList);
        append(creditCardNumField);
        append(fillOutLater);
        append(done);

        exitCommand = new Command(uiController.getString(UIConstants.EXIT), 
                                  Command.EXIT, 5);
        doneCommand = new Command(uiController.getString(UIConstants.DONE), 
                                  Command.ITEM, 5);

        done.setDefaultCommand(doneCommand);
        done.setItemCommandListener(this);
        addCommand(exitCommand);
        addCommand(doneCommand);
        setCommandListener(this);
    }

    public void init(AccountInfo accountInfo) {
        this.accountInfo = accountInfo;
    } 

    public void commandAction(Command command, Item item) {
        if (command == doneCommand) {
            checkDoneCommand();
        } 
    } 

    public void commandAction(Command command, Displayable displayable) {
        if (command == exitCommand) {
            uiController.exitRequested();
        } else if (command == doneCommand) {
            checkDoneCommand();
        } 
    } 

    private void checkDoneCommand() {
        if (!fillOutLater.isSelected(0)) {
            String cardNum = creditCardNumField.getString();

            if (cardNum == null || cardNum.length() == 0) {
                uiController.showErrorAlert(uiController.getString(UIConstants.CREDITCARD_NUM_BAD_LENGTH));

                return;
            } 

            // Set the information.
            String month = 
                monthList.getString(monthList.getSelectedIndex()).substring(0, 
                                    3);
            ;
            String year = yearList.getString(yearList.getSelectedIndex());

            accountInfo.setCreditCardExpirationDate(month + year);
            accountInfo.setCreditCardNumber(cardNum);
            uiController.billingInfoSaved(accountInfo);
        } else {
            uiController.mainMenuRequested();
        } 
    } 

    private void createItems() {
        cardList = Utils.newCardPopUp(uiController);
        monthList = Utils.newMonthPopUp(uiController);
        yearList = Utils.newYearPopUp(uiController);
        fillOutLater = new ChoiceGroup(null, Choice.MULTIPLE);

        fillOutLater.append(uiController.getString(UIConstants.FILL_OUT_LATER), 
                            null);

        expDateMessage = 
            new StringItem(uiController.getString(UIConstants.EXPIR_DATE), 
                           null);
        done = new StringItem(uiController.getString(UIConstants.DONE), null, 
                              Item.BUTTON);

        done.setLayout(Item.LAYOUT_CENTER);

        creditCardNumField = 
            new TextField(uiController.getString(UIConstants.CARD_NUM), null, 
                          19, TextField.NUMERIC);
    } 

}

