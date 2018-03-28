package com.enterprisej2me.HttpConnections;
 
import javax.microedition.io.*;

public class BasicAuthHandler implements Handler {
 
  private String username;
  private String password;
 
  public BasicAuthHandler (String u, String p) {
    username = u;
    password = p;
  }
 
  public void prepareHeaders(HttpConnection c) throws Exception {
    String s = encode(username + ":" + password);
    c.setRequestProperty("Authorization", "Basic " + s);
  }
 
  public boolean processHeaders (HttpConnection c) throws Exception {
    // Do nothing. Do not respond to the challenge
    return false;
  }

  // Base64 encoding.
  //
  // This implementation is adopted from Kenneth Ballard's
  // HttpClient package. Released under LGPL.
  private String encode(String d) {
    String c = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" +
               "abcdefghijklmnopqrstuvwxyz" +
               "0123456789+/";
 
    byte [] code = c.getBytes();
    byte [] s = d.getBytes();
 
    int x;
    int y = d.length() - (d.length() % 3);

    byte [] coded = new byte[4];
    String dest = "";
 
    for(x = 0; x < y; x += 3) {
      coded[3] = code[s[x + 2] % 64];
      coded[0] = code[s[x] >> 2];

      coded[1] = new Integer((s[x] % 4) << 4).byteValue();
      coded[1] += s[x + 1] >> 4;
      coded[1] = code[coded[1]];
 
      coded[2] = new Integer((s[x + 1] % 16) << 2).byteValue();
      coded[2] += s[x + 2] / 64;
      coded[2] = code[coded[2]];
 
      dest += new String(coded);
    }
 
    x = y;
 
    if(s.length % 3 == 0) return dest;
 
    if(s.length % 3 == 1) {
      coded[2] = '=';
      coded[3] = '=';

      coded[0] = code[s[x] >> 2];
      coded[1] = code[new Integer((s[x] % 4) << 4).byteValue()];
 
      dest += new String(coded);
    }
 
    if(s.length % 3 == 2) {
      coded[3] = '=';
 
      coded[0] = code[s[x] >> 2];
      coded[1] = new Integer((s[x] % 4) << 4).byteValue();
      coded[1] += s[x + 1] >> 4;
      coded[1] = code[coded[1]];
 
      coded[2] = code[new Integer((s[x + 1] % 16) << 2).byteValue()];
 
      dest += new String(coded);
    }
 
    return dest;
  }
}
