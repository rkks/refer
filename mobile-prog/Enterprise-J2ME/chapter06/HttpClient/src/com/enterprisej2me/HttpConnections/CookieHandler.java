package com.enterprisej2me.HttpConnections;

import java.io.*;
import java.util.*;
import javax.microedition.io.*;

public class CookieHandler implements Handler {

  private static Vector cookies;
  private static Vector domains;

  public CookieHandler() {
    cookies = new Vector ();
    domains = new Vector ();
  }

  public void prepareHeaders(HttpConnection c) throws Exception {
    String url = c.getURL ();
    addCookie(c, url);
  }

  public boolean processHeaders (HttpConnection c) throws Exception {
    getCookie(c);
    return false;
  }

  // Remove all cookies.
  public void removeCookies() throws Exception {
    cookies = new Vector ();
    domains = new Vector ();
    return;
  }

    
  // Retreive cookies from the connection header and save
  // them with domain information
  private void getCookie(HttpConnection c) throws Exception {
    int k = 0;
    // Iterate through connection headers and find "set-cookie" fields.
    while (c.getHeaderFieldKey(k) != null) {
      String key = c.getHeaderFieldKey(k);
      String value = c.getHeaderField(k);
      if (key.equals("set-cookie")) {
        // Parse the header and get the cookie.
        int j = value.indexOf(";");
        String cValue = value.substring(0, j);

        // We set the domain default to the current server.
        String dValue = c.getHost();
        if ( dValue == null ) {
          // If there is no valid domain, 
          // we do not keep the cookie.
        } else {
          // All upper case for easy comparision in the future.
          dValue = dValue.toUpperCase();
          
          cookies.addElement(cValue);
          domains.addElement(dValue);
        }
System.out.println(dValue + " " + cValue);
      }
      k++;
    }
    return;
  }

  // This method matches cookies with the domain
  // of the connection. The matched cookies are set into the
  // headers of the connection.
  private void addCookie(HttpConnection c, String url) throws Exception {

    // String variable domain stores the domain of of the current url.
    String domain;
    // Find the "/" or ":" after "http://"
    int indexSlash = url.indexOf("/", 7); 
    int indexColon = url.indexOf(":", 7); 
    if ( (indexSlash == -1) && (indexColon == -1) ) {
      // If neither is found, domain ends at the end of the URL.
      domain = url.substring(7, url.length());
    } else if ( indexSlash == -1 ) {
      // If not slash is found but a colon is found, the domain
      // name ends at the colon.
      domain = url.substring(7, indexColon);
    } else if ( indexColon == -1 ) {
      // If not colon is found but a slash is found, the domain
      // name ends at the slash.
      domain = url.substring(7, indexSlash);
    } else if ( (indexSlash < indexColon) ) {
      // If both are found but slash appears before colon,
      // the domain name ends at the slash.
      domain = url.substring(7, indexSlash);
    } else {
      // If both are found but colon appears before slash,
      // the domain name ends at the colon.
      domain = url.substring(7, indexColon);
    }
    domain = domain.toUpperCase();

System.out.println("Doamin is " + domain);

    StringBuffer buff = new StringBuffer();
    for (int i = 0; i < domains.size(); i++) {
      String cookieDomain = (String) domains.elementAt(i);
      if ( domain.endsWith( cookieDomain ) ) {
        buff.append( (String) cookies.elementAt(i) );
        buff.append("; ");
      }
    }

    // If we do have cookies to send, set the composed string into 
    // "cookie" header.
    String cookieStr = buff.toString();
    if ( cookieStr == null || cookieStr.equals("") ) {
      // Ignore
    } else {
      c.setRequestProperty( "cookie", cookieStr );
    }
System.out.println("cookie " + cookieStr );

    return;
  }

}
