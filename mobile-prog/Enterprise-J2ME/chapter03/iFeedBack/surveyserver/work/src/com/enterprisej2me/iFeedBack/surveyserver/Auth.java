package com.enterprisej2me.iFeedBack.surveyserver;

import java.io.*;
import org.ksoap.*;
import org.ksoap.transport.*;
import org.kxml.*;
import org.kxml.io.*;
import org.kxml.parser.*;

public class Auth {

  private static String endPointURL = 
                 "http://localhost:8080/iFeedBackSSO/server";

  public Auth () {
  }
 
  public static boolean authenticate (String token) {
   try {
      SoapObject method = new SoapObject("urn:enterprisej2me/iFeedBackSSO/Server",
                                         "authenticate");
      method.addProperty("String_1", token );
 
      HttpTransportSE rpc = new HttpTransportSE(endPointURL, "\"\"");
      boolean succ = ((Boolean) rpc.call (method)).booleanValue();
      return succ;

    } catch (Exception e) {
      e.printStackTrace();
      return false;
    }
  }
}
