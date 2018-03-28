

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
 * $Id: ProgressObserverUI.java,v 1.1 2003/04/05 00:41:51 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import com.sun.j2me.blueprints.smartticket.client.midp.util.ProgressObserver;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

public class ProgressObserverUI extends Form implements ProgressObserver, 
        CommandListener {
    private UIController uiController;
    private static final int GAUGE_MAX = 8;
    private static final int GAUGE_LEVELS = 4;
    int current = 0;
    Gauge gauge;
    Command stopCommand;
    boolean stoppable;
    boolean stopped;

    public ProgressObserverUI(UIController uiController) {
        super("");

        gauge = new Gauge("", false, GAUGE_MAX, 0);
        stopCommand = new Command(uiController.getString(UIConstants.STOP), 
                                  Command.STOP, 10);

        append(gauge);
        setCommandListener(this);
    }

    public void init(String note, boolean stoppable) {
        gauge.setValue(0);
        setNote(note);
        setStoppable(stoppable);

        stopped = false;
    } 

    public void setNote(String note) {
        setTitle(note);
    } 

    public boolean isStoppable() {
        return stoppable;
    } 

    public void setStoppable(boolean stoppable) {
        this.stoppable = stoppable;

        if (stoppable) {
            addCommand(stopCommand);
        } else {
            removeCommand(stopCommand);
        } 
    } 

    /**
     * Indicates whether the user has stopped the progress. This message should be called before calling update.
     */
    public boolean isStopped() {
        return stopped;
    } 

    public void updateProgress() {
        current = (current + 1) % GAUGE_LEVELS;

        gauge.setValue(current * GAUGE_MAX / GAUGE_LEVELS);
    } 

    public void commandAction(Command c, Displayable d) {
        if (c == stopCommand) {
            stopped = true;
        } 
    } 

}

