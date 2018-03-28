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
package de.trantor.mail.demo.list;

import de.trantor.mail.*;

/**
 * Implements a very tiny mailing list processor. This class has been used in
 * the Dr. Dobbs 06/2002 article about Mail4ME.
 */
public class MailingList {
    
    static String pop3Host = "pop.yourisp.com";
    static String pop3User = "user0815";
    static String pop3Pass = "secret";
    
    static String smtpHost = "smtp.yourisp.com";
    
    static String listMachine = "localhost";
    static String listOwner = "owner@yourlist.com";
    
    static String[] users = {"duke@sun.com", "clippy@microsoft.com"};
    
    static boolean isValidUser(Message message) throws Exception {
        String sender = message.getHeaderValue("From");
        
        sender = Message.getMachineAddress(sender).toLowerCase();
        
        for (int i = 0; i < users.length; i++) {
            if (users[i].equals(sender)) return true;
        }
        
        return false;
    }
    
    static void badMessage(Message message, String reason) throws Exception {
        String sender = message.getHeaderValue("From");
        
        Message reply = new Message(listOwner, sender, "Error");
        reply.addBodyLine("Sorry, " + Message.getDisplayAddress(sender));
        reply.addBodyLine("");
        reply.addBodyLine("your message could not be delivered because");
        reply.addBodyLine(reason);
        reply.addBodyLine("");
        reply.addBodyLine("Regards, " + listOwner);
        
        SmtpClient smtp = new SmtpClient(listMachine);
        smtp.open(smtpHost);
        smtp.sendMessage(reply);
        smtp.close();
    }
    
    static void goodMessage(Message message) throws Exception {
        Envelope envelope = new Envelope(message, false);
        envelope.setSender(listOwner);
        
        for (int i = 0; i < users.length; i++) {
            envelope.addRecipient(users[i]);
        }
        
        SmtpClient smtp = new SmtpClient(listMachine);
        smtp.open(smtpHost);
        smtp.sendMessage(envelope);
        smtp.close();
    }
    
    static boolean isTextOnly(MimeDecoder mime) {
        int count = mime.getPartCount();
        
        if (count != 0) {
            for (int i = 0; i < count; i++) {
                if (!isTextOnly(mime.getPart(i))) return false;
            }
        }
        else {
            if (!mime.getType().toLowerCase().equals("text/plain")) return false;
        }
        
        return true;
    }
    
    public static void main(String[] args) throws Exception {
        Pop3Client pop3 = new Pop3Client();
        pop3.open(pop3Host, pop3User, pop3Pass);
        
        for (int i = 0; i < pop3.getMessageCount(); i++) {
            Message message = pop3.getMessage(i);
            
            if (!isValidUser(message)) {
                badMessage(message, "you are not subscribed to the list.");
            }
            else if (!isTextOnly(new MimeDecoder(message))) {
                badMessage(message, "the list allows plain text messages only.");
            }
            else {
                goodMessage(message);
            }
            
            pop3.removeMessage(i);
        }
        
        pop3.close();
    }
}
