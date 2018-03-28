/******************************************************************************
 * Mail4ME - Mail for the Java 2 Micro Edition
 *
 * A lightweight, J2ME- (and also J2SE-) compatible package for sending and
 * receiving Internet mail messages using the SMTP and POP3 protocols.
 *
 * Copyright (c) 2000-2002 Jörg Pleumann <joerg@pleumann.de>
 *
 * Mail4ME is part of the EnhydraME family of projects. See the following web
 * sites for more information:
 *
 * -> http://mail4me.enhydra.org
 * -> http://me.enhydra.org
 *
 * Mail4ME is distributed under the Enhydra Public License (EPL), which is
 * discussed in great detail here:
 *
 * -> http://www.enhydra.org/software/license/index.html
 *
 * Have fun!
 ******************************************************************************/
package de.trantor.mail.demo.j2me;

import java.util.Vector;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

/**
 * Represents the screen that shows a list of all messages currently in the
 * POP3 inbox. Currently not very well documented due to lack of time.
 */
public class InboxScreen extends List {
    
    public static Command READ     = new Command("Read", Command.SCREEN, 1);
    public static Command WRITE    = new Command("Write", Command.SCREEN, 2);
    //public static Command REPLY    = new Command("Reply", Command.SCREEN, 3);
    //public static Command FORWARD  = new Command("Forward", Command.SCREEN, 4);
    public static Command DELETE   = new Command("Delete", Command.SCREEN, 5);
    // public static Command REFRESH = new Command("Refresh", Command.SCREEN, 6);
    //public static Command SETUP    = new Command("Setup", Command.SCREEN, 7);
    public static Command INFO = new Command("Info", Command.SCREEN, 8);
    public static Command EXIT = new Command("Exit", Command.SCREEN, 9);
    
    private static Image image;
    
    public InboxScreen(MailMIDlet midlet) {
        super("Inbox", Choice.IMPLICIT);
        
        addCommand(READ);
        addCommand(WRITE);
        //addCommand(REPLY);
        //addCommand(FORWARD);
        addCommand(DELETE);
        //addCommand(REFRESH);
        //addCommand(SETUP);
        addCommand(INFO);
        addCommand(EXIT);
        
        setCommandListener(midlet);
        
        try {
            image = Image.createImage("/Icon.png");
        }
        catch (Exception e) {
        }
    }
    
    public void setMessages(Vector messages) {
        for (int i = size() - 1; i >= 0; i--) {
            delete(i);
        }
        
        for (int i = 0; i < messages.size(); i++) {
            /**
             * BUG in Siemens SDK:
             */
            // System.out.println("Inserting element at position 0 of javax.microedition.lcdui.List");
            
            // Workaround
            if (i == 0) {
                append((String)messages.elementAt(i), image);
            }
            else {
                insert(0, (String)messages.elementAt(i), image);
            }
            // System.out.println("Ok");
        }
    }
    
    public int getMessageIndex() {
        return getSelectedIndex();
    }
}