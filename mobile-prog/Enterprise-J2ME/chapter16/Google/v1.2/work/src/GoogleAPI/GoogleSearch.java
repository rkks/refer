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
package GoogleAPI;
 
import java.io.*;
import java.util.*;
import javax.microedition.midlet.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
 
import org.ksoap.*;
import org.ksoap.transport.*;
import org.ksoap.marshal.*;
import org.kxml.*;
import org.kxml.io.*;
import org.kxml.parser.*;

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
 
    // Prepare SOAP RPC call object.
    HttpTransport rpc = new HttpTransport(endPointURL, "\"\"");
    // Google uses 1999 SOAP standards.
    ClassMap cm = new ClassMap(Soap.VER10);
    // Regiter Marshal in ClassMap and use that ClassMap for parser.
    Marshal mb = new MarshalBase64 ();
    mb.register (cm);
    rpc.setClassMap (cm);
    // Conduct RPC call through HTTP and directly get results
    byte [] cache = (byte []) rpc.call (method);
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

    // Prepare SOAP RPC call object.
    HttpTransport rpc = new HttpTransport(endPointURL, "\"\"");
    // Google uses 1999 SOAP standards.
    ClassMap cm = new ClassMap(Soap.VER10);
    rpc.setClassMap (cm);
    // Conduct RPC call through HTTP and get results
    SoapObject so = (SoapObject) rpc.call (method);
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

    // Prepare SOAP RPC call object.
    HttpTransport rpc = new HttpTransport(endPointURL, "\"\"");
    // Google uses 1999 SOAP standards.
    ClassMap cm = new ClassMap(Soap.VER10);
    rpc.setClassMap (cm);

rpc.debug=true;


    // Conduct RPC call through HTTP and directly get results
    String spellSugg = (String) rpc.call (method);

System.out.println(rpc.requestDump);
System.out.println(rpc.responseDump);

    return spellSugg;
  }

  /** 
    * Spell check implementation with more user control.
    * This function returns the Google suggested spell.
    *
    * @param query the string to be spell corrected.
    */
  public String spellCheck2 (String query) throws Exception {
    // Prepare request message
    String value =
      "<?xml version='1.0' encoding='UTF-8'?>" +
      "<SOAP-ENV:Envelope " +
       "xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" " +
       "xmlns:xsi=\"http://www.w3.org/1999/XMLSchema-instance\" " +
       "xmlns:xsd=\"http://www.w3.org/1999/XMLSchema\">" +
        "<SOAP-ENV:Body>" +
          "<ns1:doSpellingSuggestion xmlns:ns1=\"urn:GoogleSearch\" " +
            "SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">" +
           "<key xsi:type=\"xsd:string\">" + licenseKey +
           "</key>" +
           "<phrase xsi:type=\"xsd:string\">" + query +
           "</phrase>" +
          "</ns1:doSpellingSuggestion>" +
        "</SOAP-ENV:Body>" +
      "</SOAP-ENV:Envelope>";
    byte [] requestSOAP = value.getBytes();

    // Prepare HTTP connection for SOAP RPC
    HttpConnection connection =
     (HttpConnection) Connector.open (endPointURL, Connector.READ_WRITE, true);
    connection.setRequestProperty ("SOAPAction", "\"\"");
    connection.setRequestProperty ("Content-Type", "text/xml");
    connection.setRequestProperty
        ("Content-Length", ""+requestSOAP.length);
    connection.setRequestProperty ("User-Agent", "kSOAP/1.0");
    connection.setRequestMethod (HttpConnection.POST);
 
    // Send query string to endPointURL
    OutputStream os = connection.openOutputStream ();
    os.write (requestSOAP, 0, requestSOAP.length);
    os.close ();
 
    // Get and parse response SOAP message
    InputStream is = connection.openInputStream ();
    InputStreamReader reader = new InputStreamReader (is);
    XmlParser xp = new XmlParser (reader);
    // Google uses 1999 SOAP standards.
    SoapEnvelope responseEnvelope = new SoapEnvelope(new ClassMap(Soap.VER10));
    responseEnvelope.parse (xp);
    is.close ();
    connection.close ();

    // Read data from the response SOAP body
    SoapObject responseBody = (SoapObject) responseEnvelope.getBody();
    String spellSugg = (String) responseBody.getProperty("return");
    return spellSugg;
  }

}
