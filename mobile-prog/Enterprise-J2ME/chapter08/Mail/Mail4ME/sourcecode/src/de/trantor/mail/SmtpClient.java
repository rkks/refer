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
import java.util.Vector;

/**
 * Encapsulates the SMTP protocol, as specified in RFC 822. This class is used
 * to send messages across the internet. After an SMTP session has been
 * established using the open() method, an arbitrary number of messages can
 * be sent using the two variants of the sendMessage() method. One variant sends
 * a message already contained in an envelope, the other one creates the
 * envelope on-the-fly. Each SMTP session should be terminated by a call to the
 * close() method.
 *
 * @see Pop3Client
 * @see Envelope
 * @see Message
 */
public class SmtpClient {

    /**
     * Holds the socket used for communication with the SMTP server.
     */
    private Connection connection;

    /**
     * Holds the hostname of the machine the SMTP client is working on.
     */
    private String localhost;

    /**
     * Constructs an instance of the SMTP client. The hostname of the local (!)
     * machine is needed during the initial handshaking of the SMTP protocol.
     * Unfortunately J2ME/MIDP provides no means to find out the name or IP of
     * the local host, so we have to specify it here to be compatible to all
     * possible environments.
     * <p>
     * Some explanation might be handy here, since the localhost parameter
     * seems to be a source of confusion: Actually, there is *some* value
     * needed during initial SMTP handshaking. The SMTP specification says that
     * this value should identify the client machine, but most servers don't
     * seem to actually use the passed value, but check the client's IP number
     * instead (if it checks something at all). Yet, it is possible that there
     * are hosts that allow access only to "known" clients based on this value.
     * So, in most cases passing null or "localhost" will be ok. If you run into
     * problems with a specific host, try to pass a sensible value here.
     */
    public SmtpClient(String localhost) {
        this(null, localhost);
    }

    public SmtpClient(Connection connection, String localhost) {
        if (connection == null) connection = Connection.getInstance();
        if (localhost == null) localhost = "localhost";

        this.connection = connection;
        this.localhost = localhost;
    }

    /**
     * Opens a connection to the given server at the well-known port 25. No
     * username or password is required, which is ok for most SMTP servers,
     * since SMTP is basically not password-protected.
     *
     * @see #open(String, int, boolean, String, String)
     * @see #close
     * @see #connected
     */
    public void open(String host) throws IOException, MailException {
        open(host, 0, false, null, null);
    }

    /**
     * Opens a connection to the given server at the given port and using
     * secure sockets, if desired. In addition, authentication is tried if
     * both the user and the pass parameters are non-null.
     * <p>
     * A note on authentication: It requires an ESMTP server that actually
     * allows authentication. Only the PLAIN method is supported, which expects
     * the username and password information to be passed in one MIME-encoded
     * string. Other methods are too costly in terms of code, so they're not
     * supported (yet).
     *
     * @see #open(String)
     * @see #close
     * @see #connected
     */
    public void open(String host, int port, boolean ssl, String user, String pass) throws IOException, MailException {
        /**
         * Terminate any open session first.
         */
        if (connected()) close();

        /**
         * Create a connection object matching the current environment (J2ME or
         * J2SE) using the factory method of Connection, then open the socket.
         */
        connection.open(host, (port == 0 ? 25 : port), ssl);

        /**
         * Swallow the initial 'hello' line from the server, then do initial
         * handshaking. If something goes wrong, close the whole session.
         */
        try {
            execute(null);

            if ((user != null) && (pass != null)) {
                execute("EHLO " + localhost);
                execute("AUTH PLAIN " + getAuthString(user, pass));
            }
            else {
                execute("HELO " + localhost);
            }
        }
        catch (MailException e) {
            connection.close();
            throw e;
        }
    }

    /**
     * Ends the SMTP session. This method should be called after access to the
     * SMTP server, in order to close the underlying socket connection and
     * thus free resources.
     *
     * @see #open
     * @see #connected
     */
    public void close() throws IOException, MailException {
        if (connected()) {
            execute("QUIT");
            connection.close();
        }
    }

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
     * Handles a request/response pair. This is a convenience method used
     * internally to handle sending a request to the SMTP server as well as
     * receiving the response. If the response starts with a result code greater
     * than or equal to 400, and thus denotes a protocol error, an exception is
     * raised to reflect it. Note that the request is only sent if it doesn't
     * equal null, while the response is always being waited for.
     *
     * @see #send
     * @see #receive
     * @see MailException
     */
    private String execute(String command) throws IOException, MailException {
        if (command != null) connection.send(command);

        String line = connection.receive();
        String result = line;
        while ((line.length() >= 4) && (line.charAt(3) == '-')) {
            line = connection.receive();
            result = result + line.substring(3);
        }

        char c = result.charAt(0);
        if ((c == '4') || (c == '5')) {
            throw new MailException(result);
        }

        return result;
    }

    /**
     * Sends a message. This is a convenience method that automatically creates
     * an envelope for the message before sending it to the SMTP server. Note
     * that the message headers must contain a valid "From:" field and at least
     * one of "To:", "CC:" or "BCC:" for the method to work properly. This is
     * because the method attempts to copy envelope addressing information from
     * the message headers.
     *
     * @see #sendMessage(de.trantor.mail.Envelope)
     * @see Envelope
     * @see Message
     */
    public void sendMessage(Message message) throws IOException, MailException {
        sendMessage(new Envelope(message, true));
    }

    /**
     * Sends a message. This method is used to send a message that is already
     * enclosed in an envelope. Note that the envelope must contain a valid
     * sender field and at least one recipient for the method to work properly.
     *
     * @see #sendMessage(de.trantor.mail.Message)
     * @see Envelope
     * @see Message
     */
    public void sendMessage(Envelope envelope) throws IOException, MailException {
        /**
         * First of all the SMTP server wants to know the message's initiator.
         */
        execute("MAIL FROM: <" + Message.getMachineAddress(envelope.getSender()) + ">");

        /**
         * Next we have to specify all the message's recipients.
         */
        for (int i = 0; i < envelope.getRecipientCount(); i++) {
            String[] list = Message.getStringElements(envelope.getRecipient(i), ',');

            for (int j = 0; j < list.length; j++) {
                execute("RCPT TO: <" + Message.getMachineAddress(list[j]) + ">");
            }
        }

        /**
         * Finally, we transmit the message headers and body. The message's internal
         * representation already contains the empty line that is used to separate
         * the header and body parts, so we don't have to insert it ourselves.
         */
        execute("DATA");
        Vector lines = envelope.getMessage().getLines();
        for (int i = 0; i < lines.size(); i++) {
            String s = (String)lines.elementAt(i);

            /**
             * Do byte-stuffing, if necessary. Byte-stuffing takes place when a
             * line starts with a dot: A second dot is prepended to the line in
             * that case, to help the SMTP server distinguish the end of a multiline
             * request (a line containing a dot only) from lines intentionally
             * starting with a dot.
             */
            if ((s.length() != 0) && (s.charAt(0) == '.')) {
                s = '.' + s;
            }

            connection.send(s);
        }

        /**
         * The whole message is terminated by a line that contains a single dot
         * only.
         */
        execute(".");
    }

    /**
     * Queries whether the SMTP client is in debug mode or not.
     */
    public boolean getDebug() {
        return connection.getDebug();
    }

    /**
     * Controls printing of protocol debugging information to standard output.
     */
    public void setDebug(boolean value) {
        connection.setDebug(value);
    }

    /**
     * Encodes SMTP authentication data. This method is used to derive the
     * MIME-encoded authentication string for secure SMTP servers.
     * <p>
     * Contains code from Stefan Haustein's KObjects library (www.kobjects.org)
     * used by permission.
     *
     * @see #open(String, int, boolean, String, String)
     */
    private static String getAuthString(String user, String pass) {

        char[] charTab = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/".toCharArray();

        byte[] data = ("\000" + user + "\000" + pass).getBytes();

        StringBuffer buf = new StringBuffer();

        int start = 0;
        int len = data.length;

        int end = len - 3;
        int i = start;

        while (i <= end) {
            int d = ((((int) data [i]) & 0x0ff) << 16)
                | ((((int) data [i+1]) & 0x0ff) << 8)
                | (((int) data [i+2]) & 0x0ff);

            buf.append(charTab [(d >> 18) & 63]);
            buf.append(charTab [(d >> 12) & 63]);
            buf.append(charTab [(d >> 6) & 63]);
            buf.append(charTab [d & 63]);

            i += 3;
        }

        if (i == start + len - 2) {
            int d = ((((int) data [i]) & 0x0ff) << 16)
                | ((((int) data [i+1]) & 255) << 8);

            buf.append(charTab [(d >> 18) & 63]);
            buf.append(charTab [(d >> 12) & 63]);
            buf.append(charTab [(d >> 6) & 63]);
            buf.append("=");
        }
        else if (i == start + len - 1) {
            int d = (((int) data [i]) & 0x0ff) << 16;

            buf.append(charTab [(d >> 18) & 63]);
            buf.append(charTab [(d >> 12) & 63]);
            buf.append("==");
        }

        return buf.toString();
    }
}
