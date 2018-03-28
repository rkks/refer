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
import de.trantor.mail.Message;

/**
 * Represents the screen that is used to display retrieved messages. Currently
 * not very well documented due to lack of time.
 */
public class ReadScreen extends Form {
    
    private StringItem sender  = new StringItem("From:", "");
    private StringItem date    = new StringItem("Date:", "");
    private StringItem subject = new StringItem("Subject:", "");
    
    public static Command OK = new Command("Ok", Command.SCREEN, 1);
    
    public ReadScreen(MailMIDlet midlet) {
        super("Read Message");
        
        append(sender);
        append(date);
        append(subject);
        
        addCommand(OK);
        
        setCommandListener(midlet);
    }
    
    public void setSender(String sender) {
        this.sender.setText(sender);
    }
    
    public void setDate(String date) {
        this.date.setText(date);
    }
    
    public void setSubject(String subject) {
        this.subject.setText(subject);
    }
    
    public void clearBody() {
        for (int i = size() - 1; i > 2; i--) {
            delete(i);
        }
    }
    
    // setBody
    public void addBody(String line) {
        if (line.equals("")) {
            append(" ");
        }
        else {
            append(line); // new StringItem(null, line, null));
        }
    }
    
    public void addImage(Image image) {
        append(image);
    }
}