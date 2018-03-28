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

/**
 * A simple command-line application to demonstrate reading messages
 * from a POP3 server.
 */
public class DemoRead {

    public static void showMimeInfo(String prefix, MimeDecoder mime) {
	if (mime.getType() == null) {
	    System.out.println(prefix + " Type: Unknown");
	}
	else {
	    System.out.println(prefix + " Type: " + mime.getType());
	    System.out.println(prefix + " Name: " + mime.getName());
	    System.out.println(prefix + " Encoding: " + mime.getEncoding());

	    if (mime.getType().toLowerCase().startsWith("multipart/")) {
		for (int i = 0; i < mime.getPartCount(); i++) {
		    showMimeInfo(prefix + "." + i, mime.getPart(i));
		}
	    }
	}

	System.out.println("");
    }

    public static void main(String[] args) {
	if (args.length < 3) {
	    System.out.println("Usage: java de.trantor.mail.DemoRead <pop3host> <login> <password> [<number>]");
	    System.out.println("");
	    System.out.println("Specifying a message number results in that message being downloaded from the");
	    System.out.println("server. Omitting this parameter results in a list of messages being displayed.");
	    System.exit(0);
	}

	try {
	    Pop3Client pop3 = new Pop3Client();
	    pop3.setDebug(true);
	    pop3.open(args[0], args[1], args[2]);

	    try {
		if (args.length == 3) {
		    int count = pop3.getMessageCount();
		    for (int i = 0; i < count; i++) {
			Message message = pop3.getHeaders(i);
			System.out.println("Message #" + i);
			System.out.println("  From:    " + message.getHeaderValue("From"));
			System.out.println("  Subject: " + message.getHeaderValue("Subject"));
			System.out.println("  Lines:   " + message.getBodyLineCount());
		    }
		}
		else {
		    int number = Integer.parseInt(args[3]);
		    Message message = pop3.getMessage(number);
		    System.out.println("From:    " + message.getHeaderValue("From"));
		    System.out.println("Subject: " + message.getHeaderValue("Subject"));
		    System.out.println("");

		    showMimeInfo("MIME-Part 1", new MimeDecoder(message));

		    //for (int i = 0; i < message.getBodyLineCount(); i++) {
		    //  System.out.println(message.getBodyLine(i));
		    //}
		}
	    }
	    finally {
		pop3.close();
	    }
	}
	catch (Exception e) {
	    e.printStackTrace();
	}
    }
}