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
import de.trantor.mail.Message;

/**
 * Represents the screen that is used to write new messages. Currently
 * not very well documented due to lack of time.
 */
public class WriteScreen extends Form {
    
    private TextField recipient = new TextField("To:", "", 128, TextField.EMAILADDR);
    private TextField subject = new TextField("Subject:", "", 128, TextField.ANY);
    private TextField body = new TextField("Body:", "", 8192, TextField.ANY);
    
    public static Command OK = new Command("Ok", Command.SCREEN, 1);
    public static Command CANCEL = new Command("Cancel", Command.SCREEN, 2);
    
    public WriteScreen(MailMIDlet midlet) {
        super("Write Message");
        
        append(recipient);
        append(subject);
        append(body);
        
        addCommand(OK);
        addCommand(CANCEL);
        
        setCommandListener(midlet);
    }
    
    public void clear() {
        recipient.setString("");
        subject.setString("");
        body.setString("");
    }
    
    public String getRecipient() {
        return recipient.getString();
    }
    
    public String getSubject() {
        return subject.getString();
    }
    
    public String getBody() {
        return body.getString();
    }
}