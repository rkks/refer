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
package de.trantor.mail.j2se;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import de.trantor.mail.Connection;

/**
 * Provides a concrete descendent of the abstract "Connection" class that
 * can be used in the J2SE environment. It is based on java.net.Socket.
 *
 * @see de.trantor.mail.Connection
 * @see java.net.Socket
 */
public class ConnectionImpl extends Connection {

    /**
     * Holds the socket (from java.net) that is the basis of this connection.
     */
    private Socket socket;

    private InputStream input;
    
    private OutputStream output;

    /**
     * Opens a connection with the given host on the given port.
     *
     * @see #close
     */
    public void open(String host, int port, boolean ssl) throws IOException {
        if (ssl) {
            throw new IOException("Secure sockets not implemented for J2ME.");
        }

        socket = new Socket(host, port);
        socket.setSoTimeout(60000);

        input = socket.getInputStream();
        output = socket.getOutputStream();
    }

    /**
     * Closes the connection.
     *
     * @see #open
     */
    public void close() throws IOException {
        if (socket != null) {
            socket.close();
            socket = null;
        }
    }

    protected int read(byte[] buffer, int offset, int count) throws IOException {
        return input.read(buffer, offset, count);
    }
    
    protected void write(byte[] buffer, int offset, int count) throws IOException {
        output.write(buffer, offset, count);
    }
    
    public boolean connected() {
        return (socket != null);
    }
}