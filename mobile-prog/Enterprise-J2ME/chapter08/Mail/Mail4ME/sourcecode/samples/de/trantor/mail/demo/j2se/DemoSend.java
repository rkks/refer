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
package de.trantor.mail.demo.j2se;

import de.trantor.mail.*;
import java.net.InetAddress;
import java.net.UnknownHostException;

/**
 * A simple command-line application to demonstrate sending messages
 * through an SMTP server.
 */
public class DemoSend {
    
    public static void main(String[] args) {
        if (args.length != 3) {
            System.out.println("Usage: java de.trantor.mail.demo.j2seDemoSend <smtphost> <from-address> <to-address>");
            System.out.println("");
            System.out.println("Sends a short message (with fixed subject and body) from the given sender to the");
            System.out.println("given recipient.");
            System.exit(0);
        }
        
        try {
            String localhost;
            try {
                localhost = InetAddress.getLocalHost().getHostAddress();
            }
            catch (UnknownHostException e) {
                localhost = "localhost";
            }
            
            SmtpClient smtp = new SmtpClient(localhost);
            // smtp.setDebug(true);
            smtp.open(args[0]);
            
            try {
                Message message = new Message(args[1], args[2], "Test message sent by Mail4ME");
                message.addBodyLine(("This space intentionally left blank."));
                smtp.sendMessage(message);
            }
            finally {
                smtp.close();
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}