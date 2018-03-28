package com.enterprisej2me.SmartPhrases.midp.SearchUtils;
 
import java.io.*;
import java.util.*;
import javax.microedition.midlet.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
 
import org.ksoap.*;
import org.ksoap.transport.*;

/**
  * A thin wrapper class over the Google SOAP Web services API.
  * Application examples see MIDlet MIDlets.GoogleTest 
  */

public class GoogleSearch {
 
  private String licenseKey, endPointURL;

  /**
    * Constructor with license key and API URL.
    *
    * @param licenseKey the license key obtained from Google.com.
    * @param endPointURL the Google web services API URL.
    */
  public GoogleSearch (String licenseKey, String endPointURL) {
    this.licenseKey = licenseKey;
    this.endPointURL = endPointURL;
  }

  /** 
    * This function returns the Google search results of a specific
    * query string from a specific index. It always returns ten
    * results (if available) from that index.
    *
    * @param query the string to be searched.
    * @param start the start index to return results.
    */
  public GoogleResults search (String query, int start) throws Exception {
    // Prepare request SOAP message in a memory object
    SoapObject method = new SoapObject("urn:GoogleSearch",
                                       "doGoogleSearch");
    method.addProperty("key", licenseKey);
    method.addProperty("q", query);
    method.addProperty("start", new Integer(start));
    method.addProperty("maxResults", new Integer(10));
    method.addProperty("filter", new Boolean(true));
    method.addProperty("restrict", "");
    method.addProperty("safeSearch", new Boolean(false));
    method.addProperty("lr", "");
    method.addProperty("ie", "latin1");
    method.addProperty("oe", "latin1");

    // Prepare SOAP RPC call object.
    HttpTransport rpc = new HttpTransport(endPointURL, "\"\"");
    // Google uses 1999 SOAP standards.
    ClassMap cm = new ClassMap(Soap.VER10);
    rpc.setClassMap (cm);
    // Conduct RPC call through HTTP and get results
    SoapObject so = (SoapObject) rpc.call (method);
    GoogleResults sr = new GoogleResults ();
    sr.estimatedTotalResultsCount = ((Integer) so.getProperty("estimatedTotalResultsCount")).intValue ();
    sr.searchTime = ((SoapPrimitive) so.getProperty("searchTime")).toString ();

    Vector items = (Vector) so.getProperty("resultElements");
    for (int i = 0; i < items.size(); i++) {
      SoapObject item = (SoapObject) items.elementAt (i);
      sr.sizes.addElement (item.getProperty ("cachedSize"));
      sr.urls.addElement (item.getProperty ("URL"));
      sr.titles.addElement (item.getProperty ("title"));
    }
    return sr;
  }

  /** 
    * Simple spell check using convenience class HttpTransport
    * This function returns the Google suggested spell.
    *
    * @param query the string to be spell corrected.
    */
  public String spellCheck (String query) throws Exception {
    // Prepare request SOAP message in a memory object
    SoapObject method = new SoapObject("urn:GoogleSearch",
                                       "doSpellingSuggestion");
    method.addProperty("key", licenseKey);
    method.addProperty("phrase", query);

    // Prepare SOAP RPC call object.
    HttpTransport rpc = new HttpTransport(endPointURL, "\"\"");
    // Google uses 1999 SOAP standards.
    ClassMap cm = new ClassMap(Soap.VER10);
    rpc.setClassMap (cm);
    // Conduct RPC call through HTTP and directly get results
    String spellSugg = (String) rpc.call (method);
    return spellSugg;
  }

}
