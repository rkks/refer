/*
 * Copyright 1999-2001 Sun Microsystems, Inc. ALL RIGHTS RESERVED
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistribution in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 *
 * Neither the name of Sun Microsystems, Inc. or the names of
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty of any
 * kind. ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND
 * WARRANTIES, INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE HEREBY
 * EXCLUDED. SUN AND ITS LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES
 * SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
 * DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN
 * OR ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR
 * FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR
 * PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF
 * LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE SOFTWARE,
 * EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * 
 * You acknowledge that Software is not designed, licensed or intended
 * for use in the design, construction, operation or maintenance of
 * any nuclear facility.
 */

// Re-packaged by Michael Juntao Yuan and Ju Long.
package com.enterprisej2me.HttpConnections;

import java.io.*;
import javax.microedition.io.*;

/**
 * Simple way of putting cookie support into the framework.  use
 * SessionConnector.open() instead of Connector.open() 
 */
public class SessionConnector {
    private SessionConnector() {}

    private static String cookie;
    
    public static HttpConnection open(String url) throws IOException {
        HttpConnection c = (HttpConnection) Connector.open(url);
        addCookie(c);
        // TBD Set these from the system props
        c.setRequestProperty("User-Agent",
                             "Profile/MIDP-1.0, Configuration/CLDC-1.0");
        c.setRequestProperty("Content-Language",
                             "en-US");
        HttpSessionConnection sc = new HttpSessionConnection(c);
        return sc;
    }

    /**
     * Open a connection and post a command.
     * The URL is opened and the command is written to the output
     * stream and the OutputStream is closed.
     *
     * @param url to open a connection for
     * @param command to POST to the stream
     * @returns an open HttpConnection
     * @exception IOException if any exception occurs
     */
    public static HttpConnection open(String url, String command)
        throws IOException
    {
        HttpConnection conn = open(url);
        conn.setRequestMethod(HttpConnection.POST);
        DataOutputStream os = null;
        command = command + '\n';
        try {
            os = conn.openDataOutputStream();
            os.write(command.getBytes());
        } finally {
            if (os != null) {
                os.close();
            }
        }

        return conn;
    }

    /**
     * Close the connection and streams.
     */
    public static void close(HttpConnection conn,
			     InputStream is,
			     OutputStream os) {
	if (is != null) {
	    try {
		is.close();
	    } catch (IOException ignore) {
		// ignore
	    }
	}

	if (os != null) {
	    try {
		os.close();
	    } catch (IOException ignore) {
		// ignore
	    }
	}

	if (conn != null) {
	    try {
		conn.close();
	    } catch (IOException ignore) {
		// ignore
	    }
	}
    }

    static void addCookie(HttpConnection c) throws IOException {
        if (cookie != null) {
            c.setRequestProperty("cookie", cookie);     
        }
    }
    
    static void getCookie(HttpConnection c) throws IOException {
        int k = 0;
        while (c.getHeaderFieldKey(k) != null) {
            String key = c.getHeaderFieldKey(k);
            String value = c.getHeaderField(k);
            if (key.equals("set-cookie")) {
                int j = value.indexOf(";");
                String cValue = value.substring(0, j);
                cookie = cValue;
            }
            k++;
        }
    }
}

class HttpSessionConnection implements HttpConnection {
    private HttpConnection c;
   
    public HttpSessionConnection(HttpConnection c) {
        this.c = c;
    }
  
    public String getURL() {
        return c.getURL();
    }

    public String getProtocol() {
        return c.getProtocol();
    }

    public String getHost() {
        return c.getHost();
    }

    public String getFile() {
        return c.getFile();
    }

    public String getRef() {
        return c.getRef();
    }
             
    public String getQuery() {
        return c.getQuery();
    }
        
    public int getPort() {
        return c.getPort();
    }
    
    public String getRequestMethod() {
        return c.getRequestMethod();
    }

    public void setRequestMethod(String method) throws IOException {
        c.setRequestMethod(method);
    }

    public String getRequestProperty(String key) {
        return c.getRequestProperty(key);
    }
     
    public void setRequestProperty(String key, String value)
        throws IOException {
        c.setRequestProperty(key, value);
    }

    public int getResponseCode() throws IOException {
        return c.getResponseCode();
    }

    public String getResponseMessage() throws IOException {
        return c.getResponseMessage();
    }

    public long getExpiration() throws IOException {
        return c.getExpiration();
    }

    public long getDate() throws IOException {
        return c.getDate();
    }

    public long getLastModified() throws IOException {
        return c.getLastModified();
    }

    public String getHeaderField(String name) throws IOException {
        return c.getHeaderField(name);
    }

    public int getHeaderFieldInt(String name, int def)
        throws IOException {
        return c.getHeaderFieldInt(name, def);
    }

    public long getHeaderFieldDate(String name, long def)
        throws IOException {
        return c.getHeaderFieldDate(name, def);
    }
    
    public String getHeaderField(int n) throws IOException {
        return c.getHeaderField(n);
    }
    
    public String getHeaderFieldKey(int n) throws IOException  {
        return c.getHeaderFieldKey(n);
    }
    
    public String getType() {
        return c.getType();
    }

    public String getEncoding() {
        return c.getEncoding();
    }

    public long getLength() {
        return c.getLength();
    }

    public void close() throws IOException {
        c.close();
    }

    public InputStream openInputStream() throws IOException {
        checkResponseCode();
        SessionConnector.getCookie(c);          
        return c.openInputStream();
    }
    
    public DataInputStream openDataInputStream() throws IOException {
        checkResponseCode();
        SessionConnector.getCookie(c);  
        return c.openDataInputStream();
    }
    
    public OutputStream openOutputStream()
        throws IOException {
        return c.openOutputStream();
    }
    
    public DataOutputStream openDataOutputStream()
        throws IOException {
        return c.openDataOutputStream();
    }

    private void checkResponseCode() throws IOException {
        int code = c.getResponseCode();
        if (code != c.HTTP_OK) {
            throw new IOException(code + "; " + c.getResponseMessage());
        }
    }
}
