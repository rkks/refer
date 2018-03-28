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
package de.trantor.mail.http;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import javax.microedition.io.*;
import de.trantor.mail.Connection;

/**
 * Provides a concrete descendent of the abstract "Connection" class that
 * can be used in any J2ME environments. It relies only on HTTP, which is
 * the only protocol that is mandatory in the Generic Connection Framework.
 * The class implements the client side of a TCP-through-HTTP tunnel
 * (which sounds somewhat absurd at first sight, but makes sense in the
 * J2ME scenario). The server part is implemented by the servlet contained
 * in the de.trantor.mail.proxy package.
 *
 * @see de.trantor.mail.Connection
 * @see javax.microedition.io.HttpConnection
 */
public class ConnectionImpl extends Connection {

    private String proxyHost;
    
    private int proxyPort;
    
    private String sessionKey;
    
    private HttpConnection connection;
    
    private InputStream input;
    
    private OutputStream output;
    
    //private int contentLength;
    
    private void openHttp(String method, String url) throws IOException {
        closeHttp();
        
        String host = "http://" + proxyHost + ":" + proxyPort + "/mail4me";

        connection = (HttpConnection)Connector.open(host + url, Connector.READ_WRITE, true);
        connection.setRequestMethod(method);
        connection.setRequestProperty("Connection", "close");
    }

    private void closeHttp() throws IOException {
        if (input != null) {
            input.close();
            input = null;
        }
        
        if (output != null) {
            output.close();
            output = null;
        }
        
        if (connection != null) {
            connection.close();
            connection = null;
        }
    }
    
    public ConnectionImpl(String proxyHost, int proxyPort) {
        this.proxyHost = proxyHost;
        this.proxyPort = proxyPort;
    }
    
    /**
     * Opens a connection with the given host on the given port.
     *
     * @see #close
     */
    public void open(String host, int port, boolean ssl) throws IOException {
        openHttp(HttpConnection.GET, "?command=open&host=" + host + "&port=" + port + "&ssl=" + ssl);
        sessionKey = receive();
        closeHttp();
    }

    /**
     * Closes the connection.
     *
     * @see #open
     */
    public void close() throws IOException {
        try {
	        openHttp(HttpConnection.GET, "?command=close&session=" + sessionKey);
	        read(new byte[0], 0, 0);
        }
        catch (Exception ignored) {
        }
        
        sessionKey = null;
    }
    
    protected int read(byte[] buffer, int offset, int count) throws IOException {
        if (input == null) {
            if (connection == null) {
                write(null, 0, 0);
            }
            
            if (output != null) {
	            output.flush();
	            output.close();
	            output = null;
            }
            
            input = connection.openInputStream();
            
            int response = connection.getResponseCode();
            if (response != HttpConnection.HTTP_OK) {
                String msg = connection.getResponseMessage();
                if (msg == null) {
                    msg = "Error " + response + " during HTTP session";
                }
                
                throw new IOException(msg);
            }
        }
        
        int actual = input.read(buffer, offset, count);
        
        if ((actual == -1) && (connection.getLength() != 0)) {
            closeHttp();
            actual = 0;
        }
        
        return actual;
    }
    
    protected void write(byte[] buffer, int offset, int count) throws IOException {
        if (output == null) {
            if (input != null) {
                input.close();
                input = null;
            }
            
            openHttp(HttpConnection.POST, "/" + sessionKey);
            output = connection.openOutputStream();
            
            //contentLength = 0;
        }
        
        if (count != 0) {
            output.write(buffer, offset, count);
            //contentLength = contentLength + count;
        }
    }
    
    public boolean connected() {
        return (sessionKey != null);
    }
}