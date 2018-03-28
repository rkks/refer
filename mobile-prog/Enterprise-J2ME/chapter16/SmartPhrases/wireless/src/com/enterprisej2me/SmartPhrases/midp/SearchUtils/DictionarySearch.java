package com.enterprisej2me.SmartPhrases.midp.SearchUtils;
 
import java.io.*;
import java.util.*;
import javax.microedition.midlet.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
 
import org.ksoap.*;
import org.ksoap.transport.*;

public class DictionarySearch {
 
  private String endPointURL;

  public DictionarySearch (String endPointURL) {
    this.endPointURL = endPointURL;
  }

  public String getWordMeaning (String query) throws Exception {
    SoapObject method = new SoapObject("", "getWordMeaning");
    method.addProperty("in0", query );
 
    // Prepare SOAP RPC call object.
    HttpTransport rpc = new HttpTransport(endPointURL, "\"\"");
    // First grand child
    return (String) rpc.call (method);
  }

  public Vector getSpellSugg (String query) throws Exception {
    SoapObject method = new SoapObject("", "getSpellSugg");
    method.addProperty("in0", query );
 
    // Prepare SOAP RPC call object.
    HttpTransport rpc = new HttpTransport(endPointURL, "\"\"");
    // First grand child
    return (Vector) rpc.call (method);
  }

}
