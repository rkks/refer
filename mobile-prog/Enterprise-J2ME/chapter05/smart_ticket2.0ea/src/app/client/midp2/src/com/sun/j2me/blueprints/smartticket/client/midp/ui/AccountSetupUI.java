

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
 * $Id: AccountSetupUI.java,v 1.3 2003/04/09 23:55:23 ro89390 Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.AccountInfo;
import com.sun.j2me.blueprints.smartticket.client.midp.model.Preferences;

public class AccountSetupUI extends Form implements CommandListener {
    private UIController uiController;
    private AccountInfo accountInfo;
    private Preferences preferences;
    private Command exitCommand;
    private Command nextCommand;
    private StringItem accountSetupMessage;
    // private TextField accountNameField;
    private TextField loginField;
    private TextField passwordField;
    private TextField verifyPasswordField;
    private TextField firstZipField;
    private TextField secondZipField;
    private ChoiceGroup autoLoginChoiceGroup;

    public AccountSetupUI(UIController uiController) {
        super(uiController.getString(UIConstants.ACCOUNT_SETUP));

        this.uiController = uiController;
        accountSetupMessage = 
            new StringItem(uiController.getString(UIConstants.ACCOUNT_SETUP_MESSAGE), 
                           null);

        /*
         * accountNameField    = new TextField(uiController.getString(
         * UIConstants.ACCOUNT_NAME),
         * null, 12, TextField.ANY);
         */
        loginField = new TextField(uiController.getString(UIConstants.LOGIN), 
                                   null, 12, TextField.ANY);
        passwordField = 
            new TextField(uiController.getString(UIConstants.PASSWORD), null, 
                          20, TextField.PASSWORD);
        verifyPasswordField = 
            new TextField(uiController.getString(UIConstants.RE_ENTER_PASSWORD), 
                          null, 20, TextField.PASSWORD);
        firstZipField = 
            new TextField(uiController.getString(UIConstants.FIRST_ZIP), 
                          null, 5, TextField.NUMERIC);
        secondZipField = 
            new TextField(uiController.getString(UIConstants.SECOND_ZIP), 
                          null, 5, TextField.NUMERIC);

        append(accountSetupMessage);
        // append(accountNameField);
        append(loginField);
        append(passwordField);
        append(verifyPasswordField);
        append(firstZipField);
        append(secondZipField);

        autoLoginChoiceGroup = new ChoiceGroup(null, ChoiceGroup.MULTIPLE, 
                                               new String[] {
            // uiController.getString(UIMessageCodes.SIGN_IN_AUTOMATICALLY)
            "Sign in automatically"
        }, null);

        append(autoLoginChoiceGroup);

        nextCommand = new Command(uiController.getString(UIConstants.NEXT), 
                                  Command.OK, 5);
        exitCommand = new Command(uiController.getString(UIConstants.EXIT), 
                                  Command.EXIT, 5);

        addCommand(nextCommand);
        addCommand(exitCommand);
        setCommandListener(this);
    }

    public void init(AccountInfo accountInfo, Preferences preferences) {
        this.accountInfo = accountInfo;
        this.preferences = preferences;
    } 

    public void commandAction(Command command, Displayable displayable) {
        if (command == exitCommand) {
            uiController.exitRequested();
        } else if (command == nextCommand) {
            // String accountName = null;
            String login = null;
            String password = null;
            String verifyPassword = null;
            String firstZip = null;
            String secondZip = null;

            /*
             * accountName = accountNameField.getString();
             * if (accountName == null || accountName.length() == 0) {
             * uiController.showErrorAlert(
             * uiController.getString(UIConstants.ACCOUNTNAME_BAD_LENGTH));
             * return;
             * }
             */
            login = loginField.getString();

            if (login == null || login.length() < UIConstants.LOGIN_LENGTH) {
                uiController.showErrorAlert(uiController.getString(UIConstants.LOGIN_BAD_LENGTH));

                return;
            } 

            password = passwordField.getString();

            if (password == null 
                    || password.length() < UIConstants.PASSWORD_LENGTH) {
                uiController.showErrorAlert(uiController.getString(UIConstants.PASSWORD_BAD_LENGTH));

                return;
            } 

            verifyPassword = verifyPasswordField.getString();

            if (verifyPassword == null ||!verifyPassword.equals(password)) {
                uiController.showErrorAlert(uiController.getString(UIConstants.PASSWORDS_DO_NOT_MATCH));

                return;
            } 

            firstZip = firstZipField.getString();

            if (firstZip == null 
                    || firstZip.length() != UIConstants.ZIP_LENGTH) {
                uiController.showErrorAlert(uiController.getString(UIConstants.ZIP_CODE_BAD_LENGTH));

                return;
            } 

            secondZip = secondZipField.getString();

            // it's not necessary to have 2nd zip code, but it's required to
            // be 5 characters long
            if (secondZip != null && secondZip.length() != 0 
                    && secondZip.length() != UIConstants.ZIP_LENGTH) {
                uiController.showErrorAlert(uiController.getString(UIConstants.ZIP_CODE_BAD_LENGTH));

                return;
            } 

            // Set the information.
            // accountInfo.setAccountName(accountName);
            accountInfo.setUserName(login);
            accountInfo.setPassword(password);
            preferences.setDefaultZipCode(firstZip);
            preferences.setSilentlyLoggingIn(autoLoginChoiceGroup.isSelected(0));

            if (secondZip != null) {
                preferences.setSecondZipCode(secondZip);
            } 

            uiController.accountSetupSaved(accountInfo, preferences);
        } 
    } 

}

