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

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

/**
 * Represents an info/about screen for the MIDlet. Currently
 * not very well documented due to lack of time.
 */
public class InfoScreen extends Form {
    
    public static Command OK = new Command("Ok", Command.SCREEN, 1);
    
    public InfoScreen(MailMIDlet midlet) {
        super("Info");
        
        append(new StringItem("About:", "Mail4ME Demo MIDlet\nVersion 1.0.5\n(c) 2000-2002 J.Pleumann\n"));
        append(new StringItem("Contact:", "<joerg@pleumann.de>\n"));
        append(new StringItem("Homepage:", "mail4me.enhydra.org\n"));
        append(new StringItem("Java VM:", System.getProperty("microedition.configuration") + ", " +
        Runtime.getRuntime().freeMemory() / 1024 + "K free"));
        
        addCommand(OK);
        
        setCommandListener(midlet);
    }
}