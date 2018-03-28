

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
 * $Id: ChangePasswordUI.java,v 1.1 2003/04/05 00:41:51 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.AccountInfo;

public class ChangePasswordUI extends Form implements CommandListener {
    private UIController uiController;
    private AccountInfo accountInfo;
    private Command saveCommand;
    private Command cancelCommand;
    private StringItem accountNameField;
    private StringItem loginField;
    private TextField passwordField;
    private TextField verifyPasswordField;

    public ChangePasswordUI(UIController uiController) {
        super(uiController.getString(UIConstants.CHANGE_PASSWORD_TITLE));

        this.uiController = uiController;

        createItems();
        append(accountNameField);
        append(loginField);
        append(passwordField);
        append(verifyPasswordField);

        saveCommand = new Command(uiController.getString(UIConstants.SAVE), 
                                  Command.OK, 5);
        cancelCommand = 
            new Command(uiController.getString(UIConstants.CANCEL), 
                        Command.CANCEL, 5);

        addCommand(saveCommand);
        addCommand(cancelCommand);
        setCommandListener(this);
    }

    public void init(AccountInfo accountInfo) {
        this.accountInfo = accountInfo;

        // accountNameField.setText(accountInfo.getAccountName());
        loginField.setText(accountInfo.getUserName());
    } 

    public void commandAction(Command command, Displayable displayable) {
        if (command == cancelCommand) {
            uiController.mySettingsRequested();
        } else if (command == saveCommand) {
            String password = null;
            String verifyPassword = null;

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

            accountInfo.setPassword(password);
            uiController.accountInfoSaved(accountInfo);
        } 
    } 

    private void createItems() {
        accountNameField = 
            new StringItem(uiController.getString(UIConstants.ACCOUNT_NAME), 
                           null);
        loginField = new StringItem(uiController.getString(UIConstants.LOGIN), 
                                    null);
        passwordField = 
            new TextField(uiController.getString(UIConstants.PASSWORD), null, 
                          20, TextField.PASSWORD);
        verifyPasswordField = 
            new TextField(uiController.getString(UIConstants.RE_ENTER_PASSWORD), 
                          null, 20, TextField.PASSWORD);
    } 

}

