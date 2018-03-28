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
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Is the abstract base class for socket connections used inside the SMTP,
 * POP3 and IMAP protocols of the mail package. The rationale for using this class
 * is the difference in how networking is handled in J2ME and J2SE: While J2SE
 * has java.net.Socket, J2ME uses the Generic Connection Framework with its
 * javax.io.Connector class as a central means to open sockets or HTTP
 * connection. Unfortunately, both methods are totally incompatible with each
 * other, so a common abstraction has to be found to make the mail package
 * work in both environments.
 * <p>
 * The Connection class is such an abstraction. It provides abstract
 * versions of methods that open and close sockets, read from them and write
 * to them. It also provides a static factory method that is able to instantiate
 * one of several sub-classes of Connection that match the different run-time
 * environments. These sub-classes are all called ConnectionImpl, and they are
 * loaded "by name" from the j2me, j2se, or http packages, respectively. This
 * is the only way to get rid of compile-time dependencies on these classes.
 */
public abstract class Connection {

    /**
     * Byte array holding carriage return and line feed
     */
    private static final byte[] CRLF = new byte[] {13, 10};
    
    /**
     * If true, protocol debugging information is written to standard output.
     */
    private boolean debug;

    /**
     * Provides a buffer used for incoming data.
     */
    private byte[] buffer = new byte[128];

    /**
     * Holds the actual number of bytes in the buffer.
     */
    private int count;

    /**
     * Holds the position of the next byte to be read from the buffer.
     */
    private int position;

    /**
     * Opens a connection. Concrete descendents of Connection override this method
     * to actually do something useful.
     */
    public abstract void open(String host, int port, boolean ssl) throws IOException;

    /**
     * Closes a connection. Concrete descendents of Connection override this
     * method to actually do something useful.
     */
    public abstract void close() throws IOException;

    /**
     * Writes to the socket.
     */
    protected abstract void write(byte[] buffer, int offset, int count) throws IOException;
    
    /**
     * Reads from the socket.
     */
    protected abstract int read(byte[] buffer, int offset, int count) throws IOException;

    /**
     * Queries the current status of the connection.
     */
    public abstract boolean connected();

    /**
     * Creates a new, concrete connection object for either J2SE or J2SE,
     * depending on the current run-time environment.
     */
    public static Connection getInstance() {
        String classname;
        String platform;

        /**
         * Make use of the "microedition.platform" property to find
         * out which run-time environment is currently in use.
         */
        if (System.getProperty("microedition.configuration") != null) {
            classname = "de.trantor.mail.j2me.ConnectionImpl";
            platform = "J2ME";
        }
        else {
            classname = "de.trantor.mail.j2se.ConnectionImpl";
            platform = "J2SE";
        }

        try {
            return (Connection)Class.forName(classname).newInstance();
        }
        catch (Exception e) {
            throw new RuntimeException("Error loading connection class for '" + platform + "' (" + e.getClass().getName() + ")");
        }
    }
    
    /**
     * Sends a string to the server. This method is used internally for
     * all outgoing communication to the server. The main thing it does
     * it terminate the line with a CR/LF. If there are occurences of CR or
     * LF inside the string, the method ensures that proper CR/LF sequences
     * are sent for them, since this is what most internet protocols expect.
     *
     * @see #receive
     */
    public void send(String s) throws IOException {
        byte[] bytes = s.getBytes();
        int length = bytes.length;

        /**
         * Special case for empty strings: Only CR/LF is sent.
         */
        if (s.length() == 0) {
            if (debug) System.out.println("[SEND]");

            write(CRLF, 0, 2);
        }
        /**
         * The usual case goes here.
         */
        else {
            int i = 0;
            while (i < length) {
                int j = i;

                /**
                 * Find next occurence of a line separator or the end of the
                 * string.
                 */
                while ((j < length) && (bytes[j] != 0x0A) && (bytes[j] != 0x0D)) {
                    j++;
                }

                if (debug) System.out.println("[SEND] " + s.substring(i, j));

                /**
                 * Write the string up to there and terminate it properly.
                 */
                write(bytes, i, j - i);
                write(CRLF, 0, 2);

                /**
                 * If we stopped at a CR, ignore a possibly following LF.
                 */
                if ((j < length - 1) && (bytes[j] == 0x0D) && (bytes[j + 1] == 0x0A)) {
                    j++;
                }

                i = j + 1;
            }
        }
    }

    /**
     * Receives a string from the server. This method is used internally for
     * incoming communication from the server. The main thing it does is
     * ensuring that only complete lines are returned to the application, that is,
     * lines that were terminated at least by a CR. LFs are ignored completely.
     * Neither CRs nor LFs are returned as part of the result.
     *
     * @see #send
     */
    public String receive() throws IOException {
        /**
         * A note on how this method works and why it is designed the
         * way it is: A previous implementation tried to read multiple
         * bytes from the InputStream, searched a possible line terminator
         * in them, and appended characters to a temporary result string.
         * That didn't work very well, because the code had to deal
         * with a large number of temporary strings that grew as more
         * characters belonging to a line were read. The result was
         * a high level of heap fragmentation, which made applications
         * instable on MIDP devices (that don't provide compacting GC,
         * that is).
         * 
         * This new implementation seems to work better: Is reads
         * characters from the InputStream one-by-one, putting them
         * in a buffer of fixed size that is large enough to hold a
         * "normal" line belonging to an e-mail. When a terminator
         * is found, a new String object is created from that buffer.
         * Since the buffer is of fixed size and used during the whole
         * lifetime of the Connection object, and the String is the
         * actual String that makes it into the final Message object,
         * there are no temporary Strings needed -- at least not in the
         * common case: If a mail server tends to send whole messages
         * without line terminators (and thus disregards for example
         * the POP3 specification that dictates a maximum response of
         * 512 characters), temporary Strings will again be necessary,
         * but there's not much one can about that.
         */
        String result = "";
        boolean stop = false;

        /**
         * The "stop" flag will be set as soon as we have received
         * a complete line, that is, a line terminated by CR/LF. Until
         * this is the case, the following block is repeated.
         */
        while (!stop) {
            count = 0;

            /**
             * The inner block reads single bytes from the InputStream
             * until, again, a line terminator is read or the buffer is
             * full.
             */	        
	        while (true) {
                int actual = read(buffer, count, 1);
                
                /**
                 * If -1 is returned, the InputStream is already closed,
                 * probably because the connection is broken, or the server
                 * didn't like is. Try to close the connection, but ignore
                 * any errors that might result from this.
                 */
	            if (actual == -1) {
	                try {
	                    close();
	                }
	                catch (IOException ignored) {
	                }
	                
	                throw new IOException("Connection closed");
	            }
                
                /**
                 * If no bytes have been received, we wait a little
                 * while (by yielding processing time to other threads).
                 */
	            else if (actual == 0) {
	                try {
	                    Thread.yield();
	                }
	                catch (Exception ignored) {
	                }
	            }
                
                /**
                 * If a byte has been read, examine it and put it in the
                 * buffer.
                 */
	            else {
                    byte b = buffer[count];
                    
                    /**
                     * Ignore all CRs.
                     */
                    if (b == 0x0D) {
                        /* Ignore CRs */
                    }
                    /**
                     * Take LF as line separator.
                     */
                    else if (b == 0x0A) {
                        stop = true;
                        break;
                    }
                    /**
                     * Everything else makes it into the buffer.
                     */ 
                    else {
                        count++;
                        if (count == buffer.length) break;
                    }
	            }
	        }

            result = result + new String(buffer, 0, count);
        }
        
        if (debug) System.out.println("[RECV] " + result);

        return result;
    }

    /**
     * Controls the output of debugging information to standard output. Set it to
     * true to see all protocol information exchanged.
     *
     * @see #getDebug
     */
    public void setDebug(boolean debug) {
        this.debug = debug;
    }

    /**
     * Queries the current value of the debugging flag.
     *
     * @see #setDebug
     */
    public boolean getDebug() {
        return debug;
    }
}