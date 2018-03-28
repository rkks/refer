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
package de.trantor.mail;

/**
 * An exception class for mail errors. An instance of this class is raised
 * whenever a protocol error occurs during a POP3, SMTP or IMAP session. This
 * is the case, for example, when a given password is incorrect or when the
 * client tries to retrieve a message that doesn't exist. The getMessage()
 * method returns the server response that signaled the error.
 *
 * @see Pop3Client
 * @see ImapClient
 * @see SmtpClient
 */
public class MailException extends Exception {
    public MailException(String message) {
        super(message);
    }
}
