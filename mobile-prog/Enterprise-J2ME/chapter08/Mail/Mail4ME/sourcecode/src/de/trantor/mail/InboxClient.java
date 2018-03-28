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

import java.io.IOException;

/**
 * This class provides an abstraction for the various protocols that are used
 * to access a mailbox (currently POP3 and IMAP). The first thing an application
 * should do is create an instance of a concrete protocol using the getInstance()
 * method. After a session has been established using the open() method on this
 * instance, the number of available messages can be queried by calling the
 * getMessageCount() method, and arbitrary messages or their headers can be
 * retrieved from the mailbox using getMessage() or getHeaders(), respectively.
 * Deleting messages is possible using removeMessage(). Each session should be
 * terminated by a call to the close() method.
 * <p>
 * Note that all message numbering follows the usual Java conventions for
 * vectors. Thus message indices must be numbers ranging from 0 to
 * getMessageCount() - 1, regardless of what the conventions for the actual
 * protocols are. Implementors of new concrete protocol classes have to keep that
 * in mind, too, of course.
 * 
 * @see Pop3Client
 * @see ImapClient
 */
public abstract class InboxClient {

    /**
     * Holds the socket used for communication with the server.
     */
    protected Connection connection;

    /**
     * If true, protocol debugging information is written to standard output.
     */
    private boolean debug;

    /**
     * Initializes the Connection object belonging to this InboxClient. This
     * constructor is never called directly, but from descendant classes instead.
     */
    protected InboxClient(Connection connection) {
        if (connection == null) connection = Connection.getInstance();
        this.connection = connection;
    }

    /**
     * Opens a mailbox session. In addition to the remote host's name, username
     * and password have to be specified in order to get access to the mailbox.
     * The well-known port is used, and an insecure (non-SSL) connection is
     * opened.
     *
     * @see #open(String, int, boolean, String, String)
     * @see #close
     * @see #connected
     */
    public void open(String host, String user, String pass) throws Exception, IOException, MailException {
        open(host, 0, false, user, pass);
    }

    /**
     * Opens a mailbox session. In addition to the remote host's name, the port
     * and the comnnection type (SSL or simple TCP), the username and password
     * have to be specified in order to get access to the mailbox. Note that you
     * can pass a zero port number to use the well known port for the protocol.
     *
     * @see #open(String, String, String)
     * @see #close
     * @see #connected
     */
    public abstract void open(String host, int port, boolean ssl, String user, String pass) throws Exception, IOException, MailException;

    /**
     * Ends the mailbox session. This method should be called after access to the
     * server, in order to close the underlying socket connection and thus free
     * resources.
     *
     * @see #open
     * @see #connected
     */
    public abstract void close() throws IOException, MailException;

    /**
     * Returns true, if the client is currently connected to an SMTP server.
     *
     * @see #open
     * @see #close
     */
    public boolean connected() {
        return connection.connected();
    }

    /**
     * Queries the number of messages currently available in the mailbox.
     *
     * @see #getMessage
     * @see #getHeaders
     * @see #removeMessage
     *
     */
    public abstract int getMessageCount() throws IOException, MailException;

    /**
     * Retrieves a message from the mailbox. The method retrieves the
     * message with the given index from the server. Message numbering
     * follows the usual Java conventions for vectors. Thus the index must
     * be a number ranging from 0 to getMessageCount() - 1.
     *
     * @see #getMessageCount
     * @see #getHeaders
     */
    public abstract Message getMessage(int index) throws IOException, MailException;

    /**
     * Retrieves a message's headers from the mailbox. The method retrieves
     * the headers of the message with the given index from the server. It
     * does not retrieve any body lines, so this message is a good means to get
     * a quick catalog of the messages available in a mailbox. Message numbering
     * follows the usual Java conventions for vectors. Thus the index must be a
     * number ranging from 0 to getMessageCount() - 1.
     *
     * @see #getMessageCount
     * @see #getMessage
     */
    public abstract Message getHeaders(int index) throws IOException, MailException;

    /**
     * Queries the unique ID of a message as assigned by the server that holds
     * the mailbox. This is not the "Message-ID" field of the message header,
     * it is a value used by the server to uniqely identify messages stored
     * in the mailbox. The server guarantees that no two messages stored in the
     * mailbox will ever have the same ID value. So this value can be used to
     * find out which messages have already been downloaded and which ones have
     * not.
     * <p>
     * Message numbering follows the usual Java conventions for vectors. Thus
     * the index must be a number ranging from 0 to getMessageCount() - 1.
     *
     * @see #getMessageCount
     */
    public abstract String getUniqueId(int index) throws IOException, MailException;

    /**
     * Queries the size of a message. This method queries the size of the given
     * message in bytes.
     * <p>
     * Message numbering follows the usual Java conventions for vectors. Thus
     * the index must be a number ranging from 0 to getMessageCount() - 1.
     *
     * @see #getMessageCount
     */
    public abstract int getSize(int index) throws IOException, MailException;

    /**
     * Removes a message from the mailbox. The method deletes the message with
     * the given index from the mailbox. When deleting messages, two things
     * have to be noted:
     *
     * <ul>
     *   <li> The messages might not be immediately deleted from the server. For
     *        POP3, this usually happens when the session is closed. It is not
     *        possible to retrieve or otherwise access a deleted message any
     *        longer, though.
     *
     *   <li> Because the message is not deleted immediatly, the indices of other
     *        messages in the box don't change until the session is closed. So
     *        if you delete message #0 of two messages, there's still a message
     *        #1 in the mailbox.
     * </ul>
     *
     * Message numbering follows the usual Java conventions for vectors. Thus the
     * index must be a number ranging from 0 to getMessageCount() - 1.
     *
     * @see #getMessageCount
     */
    public abstract void removeMessage(int index) throws IOException, MailException;

    /**
     * Controls the output of debugging information to standard output. Set it to
     * true to see all protocol information exchanged.
     *
     * @see #getDebug
     */
    public void setDebug(boolean debug) {
        connection.setDebug(debug);
    }

    /**
     * Queries the current value of the debugging flag.
     *
     * @see #setDebug
     */
    public boolean getDebug() {
        return connection.getDebug();
    }
}
