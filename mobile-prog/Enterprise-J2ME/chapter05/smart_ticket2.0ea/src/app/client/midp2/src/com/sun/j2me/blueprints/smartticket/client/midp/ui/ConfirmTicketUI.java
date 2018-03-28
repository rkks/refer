

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
 * $Id: ConfirmTicketUI.java,v 1.1 2003/04/05 00:41:51 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.client.midp.util.Utils;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan.Seat;

public class ConfirmTicketUI extends Form implements CommandListener, 
        ItemCommandListener {
    private UIController uiController;
    private Command cancelCommand;
    private Command confirmCommand;
    private StringItem theater;
    private StringItem movie;
    private StringItem showTimeStr;
    private StringItem seatsStr;
    private StringItem cost;
    private StringItem totalCost;
    private StringItem placeOrderBtn;

    public ConfirmTicketUI(UIController uiController) {
        super(uiController.getString(UIConstants.CONFIRM_TITLE));

        this.uiController = uiController;

        createItems();
        append(theater);
        append(movie);
        append(showTimeStr);
        append(seatsStr);
        append(cost);
        append(totalCost);
        append(placeOrderBtn);

        confirmCommand = 
            new Command(uiController.getString(UIConstants.CONFIRM), 
                        Command.OK, 5);
        cancelCommand = 
            new Command(uiController.getString(UIConstants.CANCEL), 
                        Command.EXIT, 5);

        addCommand(confirmCommand);
        addCommand(cancelCommand);
        setCommandListener(this);
        placeOrderBtn.setDefaultCommand(confirmCommand);
        placeOrderBtn.setItemCommandListener(this);
    }

    public void init(String theaterName, String movieName, int[] showTime, 
                     Seat[] seats) {
        if (seats.length > 0) {
            theater.setText(theaterName + " - ");
            movie.setText(movieName);
            // TODO: the date is temporary hardcoded, should query current date
            showTimeStr.setText(Utils.showTimeToDate(uiController, showTime) 
                                + " 3/17/2003 " 
                                + Utils.showTimeToHourMinute(uiController, 
                                showTime));

            StringBuffer seatString = 
                new StringBuffer(uiController.getString(UIConstants.SEATS_COLON) 
                                 + "\n");

            for (int i = 0; i < seats.length; i++) {
                seatString.append(" - " 
                                  + uiController.getString(UIConstants.ROW) 
                                  + seats[i].getRowNumber() + ", " 
                                  + uiController.getString(UIConstants.SEAT) 
                                  + seats[i].getSeatNumber() + "\n");
            } 

            seatsStr.setText(seatString.toString());
            cost.setText(seats.length + " " 
                         + uiController.getString(UIConstants.TICKET_PRICE_MSG));
            totalCost.setText(uiController.getString(UIConstants.TOTAL_COST) 
                              + (seats.length * UIConstants.TICKET_PRICE) 
                              + uiController.getString(UIConstants.HANDLING));
        } 
    } 

    public void commandAction(Command command, Item item) {
        if (command == confirmCommand) {
            uiController.purchaseRequested();
        } 
    } 

    public void commandAction(Command command, Displayable displayable) {
        if (command == cancelCommand) {
            uiController.mainMenuRequested();
        } else if (command == confirmCommand) {
            uiController.purchaseRequested();
        } 
    } 

    private void createItems() {
        theater = new StringItem(null, null);
        movie = new StringItem(null, null);
        showTimeStr = new StringItem(null, null);
        seatsStr = new StringItem(null, null);
        cost = new StringItem(null, null);
        totalCost = new StringItem(null, null);
        placeOrderBtn = 
            new StringItem(null, 
                           uiController.getString(UIConstants.PLACE_ORDER), 
                           Item.BUTTON);

        placeOrderBtn.setLayout(Item.LAYOUT_CENTER);
    } 

}

