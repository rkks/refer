/*
 *
 * The contents of this file are subject to the GNU Public License
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific terms governing rights and limitations
 * under the License.
 *
 * Developed by Michael Juntao Yuan 2002.
 *
 * */
package com.enterprisej2me.GoogleAPI;
 
import java.io.*;
import java.util.*;
import javax.microedition.io.*;

import org.kxml2.io.*;
import org.ksoap2.*;
import org.ksoap2.serialization.*;
import org.ksoap2.transport.*;
 
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
    * Constructor with license key.
    *
    * @param licenseKey the license key obtained from Google.com.
    */
  public GoogleSearch (String licenseKey) {
    this.licenseKey = licenseKey;
    this.endPointURL = "http://api.google.com/search/beta2";
  }

  /**
    * This function returns a web page from Google cache.
    *
    * @param url the URL of the page Google cached.
    */
  public byte [] getCache (String url) throws Exception {
    // Prepare request SOAP message in a memory object
    SoapObject method = new SoapObject("urn:GoogleSearch",
                                       "doGetCachedPage");
    method.addProperty("key", licenseKey);
    method.addProperty("url", url);

    // Google uses 1999 SOAP standards.
    SoapEnvelope envelope = 
           new SoapSerializationEnvelope(SoapEnvelope.VER10);
    // Set the request object
    envelope.bodyOut = method;
    // Add Base 64 marshal. It is for both request and response data.
    Marshal mb = new MarshalBase64 ();
    mb.register ((SoapSerializationEnvelope) envelope);
    // Prepare SOAP RPC call object.
    HttpTransport rpc = new HttpTransport(endPointURL);
    // Make the method call (SoapAction is not needed)
    rpc.call(null, envelope);
    // Retrieve marshaled return value 
    SoapObject ret = (SoapObject) envelope.bodyIn;
    byte [] cache = (byte []) ret.getProperty("return");
    return cache;
  }

  /** 
    * This function returns the Google search results of a specific
    * query string from a specific index. It always returns ten
    * results (if available) from that index.
    *
    * @param query the string to be searched.
    * @param start the start index to return results.
    */
  public SearchResults search (String query, int start) throws Exception {
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

    // Google uses 1999 SOAP standards.
    SoapEnvelope envelope =
           new SoapSerializationEnvelope(SoapEnvelope.VER10);
    // Set the request object
    envelope.bodyOut = method;
    // Prepare SOAP RPC call object.
    HttpTransport rpc = new HttpTransport(endPointURL);
    // Make the method call (SoapAction is not needed)
    rpc.call(null, envelope);
    // Retrieve marshaled return value
    SoapObject ret = (SoapObject) envelope.bodyIn;
    SoapObject so = (SoapObject) ret.getProperty("return");

    SearchResults sr = new SearchResults ();
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

    // Google uses 1999 SOAP standards.
    SoapEnvelope envelope =
           new SoapSerializationEnvelope(SoapEnvelope.VER10);
    // Set the request object
    envelope.bodyOut = method;
    // Prepare SOAP RPC call object.
    HttpTransport rpc = new HttpTransport(endPointURL);
    // Make the method call (SoapAction is not needed)
    rpc.call(null, envelope);
    // Retrieve marshaled return value
    SoapObject ret = (SoapObject) envelope.bodyIn;
System.out.println("top level " + ret.getNamespace() + " " + ret.getName());
    String spellSugg = (String) ret.getProperty("return");
    return spellSugg;
  }

}
