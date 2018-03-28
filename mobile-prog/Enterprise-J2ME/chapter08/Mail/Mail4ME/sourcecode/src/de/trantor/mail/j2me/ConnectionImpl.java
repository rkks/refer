/******************************************************************************
 * Mail4ME - Mail for the Java 2 Micro Edition
 *
 * A lightweight, J2ME- (and also J2SE-) compatible package for sending and
 * receiving Internet mail messages using the SMTP and POP3 protocols.
 *
 * Copyright (c) 2000-2002 J�rg Pleumann <joerg@pleumann.de>
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
package de.trantor.mail.j2me;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import de.trantor.mail.Connection;

/**
 * Provides a concrete descendent of the abstract "Connection" class that
 * can be used in J2ME environments that support sockets, that is, the
 * plain TCP protocol. Is is based on the Generic Connection Framework
 * provided by the javax.microedition.io package.
 *
 * @see de.trantor.mail.Connection
 * @see javax.microedition.io.StreamConnection
 */
public class ConnectionImpl extends Connection {

    /**
     * Holds the socket connetion object (from the Generic Connection
     * Framework) that is the basis of this connection.
     */
    private StreamConnection socket;

    /**
     * Holds the InputStream associated with the socket.
     */
    private InputStream input;
    
    /**
     * Holds the OutputStream associated with the socket.
     */
    private OutputStream output;

    /**
     * Opens a connection with the given host on the given port. If the
     * ssl parameter is true, secure socket layer is used.
     *
     * @see #close
     */
    public void open(String host, int port, boolean ssl) throws IOException {
        String protocol = (ssl ? "ssocket" : "socket");
        socket = (StreamConnection)Connector.open(protocol + "://" + host + ":" + port, Connector.READ_WRITE, true);
        input = socket.openInputStream();
        output = socket.openOutputStream();
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