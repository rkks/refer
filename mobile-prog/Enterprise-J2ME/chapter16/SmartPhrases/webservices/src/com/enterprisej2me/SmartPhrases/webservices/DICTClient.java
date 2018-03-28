/*
 *  Originally from QuikDic.DICTClient
 *  Copyright (c) 2002 Daniel Tams <dantams@myrealbox.com>
 *      All Rights Reserved.
 *
 *  Modified by Michael Yuan 2002
 */
package com.enterprisej2me.SmartPhrases.webservices;

import java.io.*;
import java.net.*;
import java.util.*;

public class DICTClient {

  private static final int STANDARD_PORT = 2628;
 
  // The client identification string sent to the server upon 
  // initial connection
  private static final String CLIENT_INFO = "SmartPhrases 1.0";
 
  //  The standard commands as per RFC2229 that are sent to the DICT server.
  private static final String CLIENT_COMMAND = "CLIENT";
  private static final String DEFINE_COMMAND = "DEFINE";
  private static final String MATCH_COMMAND = "MATCH";
  private static final String SHOW_INFO_COMMAND = "SHOW INFO";
  private static final String SHOW_DATABASES_COMMAND = "SHOW DB";
  private static final String SHOW_STRATEGIES_COMMAND = "SHOW STRAT";
  private static final String SHOW_SERVER_INFO_COMMAND = "SHOW SERVER";
  private static final String QUIT_CONNECTION_COMMAND = "QUIT";

  //  The standard response codes received from the server as per RFC 2229.
  private static final String DATABASES_AVAILABLE_RESPONSE = "110";
  private static final String STRATEGIES_AVAILABLE_RESPONSE = "111";
  private static final String DATABASE_INFORMATION_RESPONSE = "112";
  private static final String SERVER_INFORMATION_RESPONSE = "114";
  private static final String DEFINITIONS_FOUND_RESPONSE = "150";
  private static final String MATCHES_FOUND_RESPONSE = "152";
  private static final String SUCCESSFUL_CONNECTION_RESPONSE = "220";
  private static final String COMMAND_COMPLETED_RESPONSE = "250";
  private static final String INVALID_DATABASE_RESPONSE = "550";
  private static final String INVALID_STRATEGY_REPONSE = "551";
  private static final String NO_MATCH_RESPONSE = "552";
  private static final String NO_DATABASES_AVAILABLE_RESPONSE = "554";
  private static final String NO_STRATEGIES_AVAILABLE_RESPONSE = "555";

  //  the port to be used
  private int port = STANDARD_PORT;

  private String DBname;

  //  This is a unique message-id similar to the specification in RFC822,
  //  which is received from the server upon successful connection. For
  //  example: <912771.9627.1019399833@miranda.org>
  private String messageID;

  //  The capabilities string received from the server upon successful
  //  connection describing its capabilities. See RFC2229 for more info.
  private String capabilities;
 
  // The string that gives a short info on the server, received upon initial
  //  conection.
  private String shortServerInfo;
 
  private BufferedReader in;
 
  private PrintWriter out;
 
  //  This is the address of the server this client is to connect to.
  private InetAddress address;
 
  //  This holds the socket connection to the server.
  private Socket socket;

  public DICTClient(String server, String dbname) 
             throws UnknownHostException, ConnectException {
    this(server, dbname, STANDARD_PORT);
  }

  public DICTClient(String server, String dbname, int port) 
             throws UnknownHostException, ConnectException {
    this.port = port;
    this.DBname = dbname;
    try {
      address = InetAddress.getByName(server);
    } catch(UnknownHostException e) {
      throw new UnknownHostException("Could not resolve host " + server);
    }
    connect();
  }

  public synchronized void close() {
    send(QUIT_CONNECTION_COMMAND);
    try {
      socket.close();
    } catch (IOException e) {
      System.err.println("Socket closing was unsuccesful");
    }
  }
 
  private void send(String command) {
    out.println(command);
  }

  private String receive() {
    try {
      String text = in.readLine();
      return text;
    } catch (IOException e) {
      e.printStackTrace();
      throw new RuntimeException("IOException in DICTClient.receive()."); 
    }
  }

  private void connect() throws ConnectException {
    String banner;
    String responseCode;
 
    try {
      socket = new Socket(address, port);
    } catch(IOException e) {
      throw new ConnectException("Could not connect to "+address+":"+port);
    }
    try {
      in = new BufferedReader(new InputStreamReader(
              socket.getInputStream(), "UTF-8"));
      out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(
              socket.getOutputStream(), "UTF-8")), true);
    } catch(IOException e) {
      close();
      e.printStackTrace();
    }

    //  This is the initial connection banner described in RFC 2229 section
    //  3.1.
    banner = receive();
    responseCode = banner.substring(0, 3);
    if (!responseCode.equals(SUCCESSFUL_CONNECTION_RESPONSE)) {
      close();
      throw new ConnectException(
              "Error connecting to server, following response received: "
              + banner);
    } else {
      int index = banner.lastIndexOf("<");
      messageID = banner.substring(index);
      capabilities = banner.substring(banner.indexOf("<"), index).trim();
      shortServerInfo = banner.substring(4, banner.indexOf("<")).trim();
    }
 
    //  Send client information
    send(CLIENT_COMMAND + " " + CLIENT_INFO);
    receive();
  }

  public String getDefinitions (String word) throws
       IllegalArgumentException, ConnectException {

    StringBuffer result = new StringBuffer ();

    String buf;
    send(DEFINE_COMMAND + " " + DBname + " \"" + word + "\"");
    buf = receive();
    if (buf == null) {
      connect();
      return getDefinitions(word);
    } else if (buf.startsWith(NO_MATCH_RESPONSE)) {
      // Do nothing
    } else if (buf.startsWith(DEFINITIONS_FOUND_RESPONSE)) {
      int itemNo = 1;
      for (buf = receive(); !buf.startsWith(COMMAND_COMPLETED_RESPONSE); buf = receive()) {
        StringBuffer def = new StringBuffer();
        String returnedWord, dict, description;
        buf = buf.substring(buf.indexOf("\"") + 1).trim();
        returnedWord = buf.substring(0, buf.indexOf("\"")).trim();
        buf = buf.substring(buf.indexOf("\"") + 1).trim();
        dict = buf.substring(0, buf.indexOf(" ")).trim();
        buf = buf.substring(buf.indexOf(" ")).trim();
        description = buf.trim();
        for (buf = receive(); !buf.startsWith(".") || buf.startsWith(".."); buf = receive()) {
          if (buf.startsWith("..")) {
            buf = buf.substring(1);
          }
          def.append(buf + "\n");
        }
//        def = removeEndNewlines(def);
        result.append("[").append(itemNo).append("]").append(" ").append(def);
        itemNo++;
      }
    } else if (buf.startsWith(INVALID_DATABASE_RESPONSE)) {
      throw new IllegalArgumentException("Invalid database: " + DBname);
    } else {
      throw new ConnectException(
           "Error connecting to server, following response received: "
           + buf);
    }
    return result.toString ();
  }

  public Vector getMatches (String strategy, String word) 
         throws IllegalArgumentException, ConnectException {
    Vector result = new Vector ();
    String buf;
    String sugg;

    send(MATCH_COMMAND + " " + DBname + " " + strategy + " \"" + word + "\"");
    buf = receive();
    if (buf == null) {
      connect();
      return getMatches(strategy, word);
    } else if (buf.startsWith(MATCHES_FOUND_RESPONSE)) {
      buf = buf.substring(buf.indexOf(" ")).trim();
      int numMatches = Integer.parseInt(buf.substring(0, buf.indexOf(" ")));
      for (int j = 0; j < numMatches; j++) {
        buf = receive();
        sugg = buf.substring(buf.indexOf(" ")).trim();
        sugg = removeQuotationMarks(sugg);
        // result.append(sugg).append(", ");
        result.add(sugg);
      }
      // next line should be "." to signify end
      buf = receive();
      if (!buf.equals(".")) {
          throw new RuntimeException("Supposed to be " + numMatches +
                      " matches only, but server returns more");
      }
      // next line should be return code 250
      buf = receive();
 
    } else if (buf.startsWith(NO_MATCH_RESPONSE)) {
      // Do nothing
    } else if (buf.startsWith(INVALID_DATABASE_RESPONSE)) {
      throw new IllegalArgumentException("Invalid database: " + DBname);
    } else if (buf.startsWith(INVALID_STRATEGY_REPONSE)) {
      throw new IllegalArgumentException("Invalid strategy: " + strategy);
    } else {
      throw new ConnectException(
            "Error connecting to server, following response received: "
            + buf);
    }
    // return result.toString ();
    return result;
  }

  private StringBuffer removeEndNewlines( StringBuffer text ) {
    int length = text.length();
    char c;
    if (length >= 1 && ((c=text.charAt(length-1)) == '\n' || c == ' ')) {
      while (length >= 2 && ((c=text.charAt(length-2)) == '\n' || c == ' ')) {
        text.deleteCharAt(length - 1);
        length--;
      }
    }
    return text;
  }

  private String removeQuotationMarks(String text) {
    text = text.trim();
    int length = text.length();
    if (text.charAt(0) == '\"' && text.charAt(length - 1) == '\"') {
      text = text.substring(1, length - 1);
    }
    return text;
  }

}
