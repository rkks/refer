package com.enterprisej2me.HttpConnections;

import java.io.*;
import java.util.*;
import javax.microedition.io.*;

public class HttpClient {

  private String url;
  private String requestMethod;
  private Vector handlers = new Vector (); 
  private int maxIteration = 3; // Max number of challenge/response cycles.
   
  public HttpClient() {}

  public void setUrl (String url) {
    this.url = url;
  }

  public void setRequestMethod (String method) {
    this.requestMethod = method;
  }

  public void setMaxIteration (int n) {
    maxIteration = n;
  }

  public void addHandler (Handler h) throws Exception {
    handlers.addElement(h);
  }
 
  public void removeAllHandlers () throws Exception {
    handlers = new Vector ();
  }
  
  public byte [] query (byte [] req) throws Exception {

    boolean needConnect = true;
    HttpConnection c = null;
    InputStream is = null;
    OutputStream os = null;
    ByteArrayOutputStream bos = null;
    byte [] buf = null;

    try {
      int currentIteration = 0;
      while (needConnect) {
        currentIteration++;
        if (currentIteration > maxIteration)
          throw new Exception("Too many Iterations");

        needConnect = false;

        if (c != null) c.close();
        if (os != null) os.close();
        c = (HttpConnection) Connector.open (url);
        c.setRequestMethod( requestMethod );

        for (int i = 0; i < handlers.size(); i++) {
          ((Handler) handlers.elementAt(i)).prepareHeaders(c);
        }
        c.setRequestProperty("User-Agent",
                "Profile/MIDP-1.0, Configuration/CLDC-1.0");
        c.setRequestProperty("Content-Language",
                "en-US");

        if ( req != null ) {
          os = c.openOutputStream ();
          os.write(req);
        }

        for (int i = 0; i < handlers.size(); i++) {
          needConnect =
                  ((Handler) handlers.elementAt(i)).processHeaders(c)
                  || needConnect;
        }
      }
      is = c.openInputStream ();
      bos = new ByteArrayOutputStream();
      buf = new byte[256];
      while (true) {
        int rd = is.read(buf, 0, 256);
        if (rd == -1) break;
        bos.write(buf, 0, rd);
      } 
      buf = bos.toByteArray();
    } finally {
      if (c != null) c.close();
      if (is != null) is.close();
      if (os != null) os.close();
      if (bos != null) bos.close();
    }
    
    return buf;
  }

}
