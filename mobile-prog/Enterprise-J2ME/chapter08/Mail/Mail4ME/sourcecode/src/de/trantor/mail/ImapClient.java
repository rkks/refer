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

import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;

/**
 * Encapsulates the IMAP v4.1 protocol as specified in RFC 2060. This class
 * provides a simple interface to a IMAP mailbox. After a session has been
 * established using the open() method, the number of available messages can be
 * queried by calling the getMessageCount() method, and arbitrary messages or
 * their headers can be retrieved from the mailbox using getMessage() or
 * getHeaders(), respectively. Deleting messages is possible using
 * removeMessage(). Each IMAP session should be terminated by a call to the
 * close() method.
 *
 * @see MailException
 * @see SmtpClient
 * @see Message
 */
public class ImapClient extends InboxClient {

    /**
     * Counts the commands executed so far in this session. Every command of an
     * IMAP session needs a unique ID that is prepended to the command line.
     */
    private int commandCount = 0;

    /**
     * Creates a new ImapClient instance.
     */
    public ImapClient() {
        super(null);
    }

    /**
     * Creates a new ImapClient instance with a given Connection instance.
     */
    public ImapClient(Connection connection) {
        super(connection);
    }

    public void open(String host, int port, boolean ssl, String user, String pass) throws IOException, MailException {
        /**
         * Terminate any open session first.
         */
        if (connected()) close();

        /**
         * Create a connection object matching the current environment (J2ME or
         * J2SE) using the factory method of Connection, then open the socket.
         */
        connection.open(host, (port == 0 ? 143 : port), ssl);

        /**
         * Swallow the initial 'hello' line from the server, then try to
         * authenticate using the given username and password. If something
         * goes wrong, close the whole session.
         */
        try {
            execute("LOGIN", user + " " + pass, null);
            execute("SELECT", "INBOX", null);
        }
        catch (MailException e) {
            connection.close();

            throw e;
        }
    }

    public void close() throws IOException, MailException {
        if (connected()) {
            execute("CLOSE", null, null);
            execute("LOGOUT", null, null);
        }

        connection.close();
    }

    /**
     * Handles a request/response pair. This is a convenience method used
     * internally to handle sending a request to the IMAP server as well as
     * receiving the response. If the response starts with a "-" sign, and thus
     * denotes a protocol error, an exception is raised to reflect it. Note that
     * the request is only sent if it doesn't equal null, while the response is
     * always being waited for.
     *
     * @see #send
     * @see #receive
     * @see MailException
     */
    private String execute(String command, String arguments, Message message) throws IOException, MailException {
        String result = null;

        String tag = "A" + commandCount++ + " ";
        connection.send(tag + command + (arguments == null ? "" : " " + arguments));

        String temp = connection.receive();
        while (!temp.startsWith(tag)) {
            if (temp.indexOf(" " + command + " ") != -1) {
                int p = temp.indexOf('(');
                int q = temp.indexOf(')', p + 1);

                if (p != -1) {
                    if (q > p) {
                        result = temp.substring(p + 1, q);
                    }
                    else if (message != null) {
                        int left = temp.indexOf('{');
                        int right = temp.indexOf('}', left);

                        receiveMessage(message, Integer.parseInt(temp.substring(left + 1, right)));
                    }
                }
            }
            temp = connection.receive();
        }

        temp = temp.substring(tag.length());
        if (temp.startsWith("BAD ") || temp.startsWith("NO ")) {
            throw new MailException(temp);
        }

        return result;
    }

    public int getMessageCount() throws IOException, MailException {
        String buffer = execute("STATUS", "INBOX (MESSAGES)", null);

        /**
         * The result of the "STAT" request should always be "+OK <#msgs> <#bytes>",
         * so we simply fetch the number between the first and the second space
         * (and keep our fingers crossed that every POP3 implementation follows the
         * RFC).
         */
        int space = buffer.indexOf(' ');

        return Integer.parseInt(buffer.substring(space + 1));
    }

    /**
     * Receives a message. This method receives a whole message from the server
     * and stores the header and body parts in the according vectors. It is able
     * to undo any byte stuffing produced by the server. It also undoes header
     * folding in a way, putting multiple header lines that belong to one
     * field into a single line of the header vector.
     * <p>
     * The method assumes that either a "RETR" or a "TOP" command has already
     * been issued, so that it can only be called from the getMessage() and
     * getHeader() methods (whom it serves as an internal helper method).
     *
     * @see #getMessage
     * @see #getHeaders
     */
    private void receiveMessage(Message message, int size) throws IOException, MailException {
        int count = 0;

        /**
         * First we read the header lines. The end of the header is denoted by
         * an empty line.
         */
        String buffer = connection.receive();
        int octets = buffer.length() + 2;

        while (!(buffer.equals(""))) {
            /**
             * Undo header folding, that is, put logical header lines that span
             * multiple physical ones into one vector entry. This eases dealing
             * with header fields a lot.
             */
            if (buffer.startsWith(" ") || buffer.startsWith("\t")) {
                message.setHeaderLine(count - 1, message.getHeaderLine(count - 1) + "\r\n" + buffer);
            }
            else {
                message.addHeaderLine(buffer);
                count++;
            }

            buffer = connection.receive();
            octets = octets + buffer.length() + 2;
        }

        /**
         * Next we read the body lines. The end of the body is denoted by a line
         * consisting only of a dot (which is the usual end of multiline respones).
         */
        while (octets < size) {
            buffer = connection.receive();
            octets = octets + buffer.length() + 2;
            message.addBodyLine(buffer);
        }
    }

    public Message getMessage(int index) throws IOException, MailException {
        Message message = new Message();
        execute("FETCH", (index + 1)+ " (RFC822)", message);
        return message;
    }

    public Message getHeaders(int index) throws IOException, MailException {
        Message message = new Message();
        execute("FETCH", (index + 1)+ " (RFC822.HEADER)", message);
        return message;
    }

    public void removeMessage(int index) throws IOException, MailException {
        execute("STORE", (index + 1) + " +FLAGS.SILENT (\\DELETED)", null);
    }

    public String getUniqueId(int index) throws IOException, MailException {
        String buffer = execute("FETCH", (index + 1) + " (UID)", null); // in Klammern?

        int space = buffer.indexOf(' ');

        return buffer.substring(space + 1);
    }

    public int getSize(int index) throws IOException, MailException {
        String buffer = execute("FETCH", (index + 1) + " (RFC822.SIZE)", null); // in Klammern?

        int space = buffer.indexOf(' ');

        return Integer.parseInt(buffer.substring(space + 1));
    }
}
