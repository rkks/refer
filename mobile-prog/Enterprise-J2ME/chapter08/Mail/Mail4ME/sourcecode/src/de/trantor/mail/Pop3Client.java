/******************************************************************************
 * Mail4ME - Mail for the Java 2 Micro Edition
 *
 * A lightweight, J2ME- (and also J2SE-) compatible package for sending and
 * receiving Internet mail messages using the SMTP and POP3 protocols.
 *
 * Copyright (c) 2000-2002 Jörg Pleumann <joerg@pleumann.de>
 *
 * Contributions: "maxLines" feature by <norbert@epublica.de>
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
 * Encapsulates the POP3 protocol as specified in RFC 1939. This class provides
 * a simple interface to a POP3 mailbox. After a session has been established
 * using the open() method, the number of available messages can be queried by calling the
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
 * @see SmtpClient
 * @see ImapClient
 * @see Message
 */
public class Pop3Client extends InboxClient {

    /**
     * Constructs an instance of the POP3 client.
     */
    public Pop3Client(){
        super(null);
    }

    /**
     * Constructs an instance of the POP3 client with a given Connection instance.
     */
    public Pop3Client(Connection connection){
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
        connection.open(host, (port == 0 ? 110 : port), ssl);

        /**
         * Swallow the initial 'hello' line from the server, then try to
         * authenticate using the given username and password. If something
         * goes wrong, close the whole session.
         */
        try {
            execute(null);
            execute("USER " + user);
            execute("PASS " + pass);
        }
        catch (MailException e) {
            connection.close();

            throw e;
        }
    }

    public void close() throws IOException, MailException {
        if (connected()) {
            execute("QUIT");
            connection.close();
        }
    }

    /**
     * Handles a request/response pair. This is a convenience method used
     * internally to handle sending a request to the POP3 server as well as
     * receiving the response. If the response starts with a "-" sign, and thus
     * denotes a protocol error, an exception is raised to reflect it. Note that
     * the request is only sent if it doesn't equal null, while the response is
     * always being waited for.
     *
     * @see #send
     * @see #receive
     * @see MailException
     */
    private String execute(String command) throws IOException, MailException {
        if (command != null) connection.send(command);

        String result = connection.receive();

        if ((result.length() > 1) && (result.charAt(0) == '-')) {
            throw new MailException(result);
        }

        return result;
    }

    public int getMessageCount() throws IOException, MailException {
        String buffer = execute("STAT");

        /**
         * The result of the "STAT" request should always be "+OK <#msgs> <#bytes>",
         * so we simply fetch the number between the first and the second space
         * (and keep our fingers crossed that every POP3 implementation follows the
         * RFC).
         */
        int space1 = buffer.indexOf(' ');
        int space2 = buffer.indexOf(' ', space1 + 1);

        return Integer.parseInt(buffer.substring(space1 + 1, space2));
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
    private Message receiveMessage() throws IOException, MailException {
        Message message = new Message();

        String line = "";

        /**
         * First we read the header lines. The end of the header is denoted by
         * an empty line.
         */
        String buffer = connection.receive();
        while (!(buffer.equals(""))) {
            /**
             * Undo byte stuffing, that is, remove a possibly leading dot from the
             * incoming lines. The server automatically inserts these to distinguish
             * the end of a message (denoted by a line with only a dot in it) from
             * intentional dots at the beginning of a line.
             */
            if ((buffer.length() > 1) && (buffer.charAt(0) == '.')) {
                buffer = buffer.substring(1);
            }

            /**
             * Undo header folding, that is, put logical header lines that span
             * multiple physical ones into one vector entry. This eases dealing
             * with header fields a lot.
             */
            if (buffer.startsWith(" ") || buffer.startsWith("\t")) {
                line = line + "\r\n" + buffer;
            }
            else {
                if ((line.length() != 0) /* && isGoodHeader(line) */) message.addHeaderLine(line);

                line = buffer;
            }

            buffer = connection.receive();
        }

        if ((line.length() != 0) /* && isGoodHeader(line) */) message.addHeaderLine(line);

        /**
         * Next we read the body lines. The end of the body is denoted by a line
         * consisting only of a dot (which is the usual end of multiline respones).
         */
        buffer = connection.receive();
        while (!(buffer.equals("."))) {
            /**
             * Again, undo byte stuffing (see above).
             */
            if ((buffer.length() > 1) && (buffer.charAt(0) == '.')) {
                buffer = buffer.substring(1);
            }

            message.addBodyLine(buffer);
            buffer = connection.receive();
        }

        return message;
    }

    /**
     * Retrieves a message from the POP3 mailbox. The method retrieves the
     * message with the given index from the POP3 server. It will only receive
     * up to "maxLines" lines. To retrieve the headers only, pass 0. To retrieve
     * the complete message regardless of its length, pass -1 for this parameter.
     * Message numbering follows the usual Java conventions for vectors. Thus
     * the index must be a number ranging from 0 to getMessageCount() - 1.
     *
     * @see #getMessageCount
     * @see #getHeaders
     * @see #getMessage(int)
     */
    public Message getMessage(int index, int maxLines) throws IOException, MailException {
        if (maxLines >= 0) {
            execute("TOP " + (index + 1) + " " + maxLines);
        }
        else {
            execute("RETR " + (index + 1));
        }
        return receiveMessage();
    }

    public Message getMessage(int index) throws IOException, MailException {
        return getMessage(index, -1);
    }

    public Message getHeaders(int index) throws IOException, MailException {
        return getMessage(index, 0);
    }

    public void removeMessage(int index) throws IOException, MailException {
        execute("DELE " + (index + 1));
    }

    public String getUniqueId(int index) throws IOException, MailException {
        String buffer = execute("UIDL " + (index + 1));

        /**
         * The result of the "UIDL" request should always be "+OK <msg> <uid>",
         * so we simply fetch the number after the second space (and keep our
         * fingers crossed that every POP3 implementation follows the RFC).
         */
        int space = buffer.indexOf(' ', 4);

        return buffer.substring(space + 1);
    }

    public int getSize(int index) throws IOException, MailException {
        String buffer = execute("LIST " + (index + 1));

        /**
         * The result of the "LIST" request should always be "+OK <msg> <bytes>",
         * so we simply fetch the number after the second space (and keep our
         * fingers crossed that every POP3 implementation follows the RFC).
         */
        int space = buffer.indexOf(' ', 4);

        return Integer.parseInt(buffer.substring(space + 1));
    }
}
