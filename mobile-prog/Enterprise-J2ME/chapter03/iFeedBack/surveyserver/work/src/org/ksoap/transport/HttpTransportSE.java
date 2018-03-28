package org.ksoap.transport;

import java.io.*;
import java.net.*;

import org.kxml.*;
import org.kxml.io.*;
import org.kxml.parser.*;

import org.ksoap.*;

// Donated to the project on 2000-09-13 by Jason Woods

/** An optional HTTP transport class for the desktop version of
    Java, suitable e.g. for applets. */


public class HttpTransportSE {

    String url;
    String soapAction = "\"\"";

    SoapEnvelope requestEnvelope = new SoapEnvelope ();
    SoapEnvelope responseEnvelope = new SoapEnvelope ();

    public boolean debug;
    public String requestDump;
    public String responseDump;

    public HttpTransportSE () {
    }


    public HttpTransportSE (String url, String soapAction) {
        this.url = url;
        this.soapAction = soapAction;
    }


    public void setUrl (String url) {
        this.url = url;
    }


    public void setSoapAction (String soapAction) {
        this.soapAction = soapAction;
    }


    public void setClassMap (ClassMap classMap) {
        requestEnvelope.setClassMap (classMap);
        responseEnvelope.setClassMap (classMap);
    }


    /** Sends the requestEnvelope and fills the
        responseEnvelope */


    public void call () throws IOException {

        ByteArrayOutputStream bos = new ByteArrayOutputStream ();
        XmlWriter xw = new XmlWriter (new OutputStreamWriter (bos));
        requestEnvelope.write (xw);
        xw.flush ();
        byte [] requestData = bos.toByteArray ();
        bos = null;
        xw = null;

        requestDump = debug ? new String (requestData) : null;
        responseDump = null;

        HttpURLConnection connection = (HttpURLConnection) new
	    URL(url).openConnection();

	connection.setUseCaches(false);
        connection.setDoOutput(true);
	connection.setDoInput (true);
	connection.setRequestProperty ("User-Agent", "kSOAP/0.99 (HttpTransportSE)");
        connection.setRequestProperty ("SOAPAction", soapAction);
	connection.setRequestProperty ("Content-Type", "text/xml");
	connection.setRequestProperty ("Connection", "close");

        connection.setRequestProperty
            ("Content-Length", ""+requestData.length);

        connection.setRequestMethod ("POST");


        OutputStream os = connection.getOutputStream();
        os.write (requestData, 0, requestData.length);
        os.close ();
        requestData = null;

        InputStream is;
	try {
	    connection.connect ();
	    is = connection.getInputStream();
	}
	catch (IOException e) {
	    is = connection.getErrorStream ();
	    if (is == null) {
		connection.disconnect ();
		throw (e);
	    }
	}

        if (debug) {
            bos = new ByteArrayOutputStream ();
            byte [] buf = new byte [256];

            while (true) {
                int rd = is.read (buf, 0, 256);
                if (rd == -1) break;
                bos.write (buf, 0, rd);
            }

	    is.close ();
            buf = bos.toByteArray ();
            responseDump = new String (buf);
            is = new ByteArrayInputStream (buf);
        }


        XmlParser xp = new XmlParser (new InputStreamReader (is));

        responseEnvelope.parse (xp);

	is.close ();
	connection.disconnect ();
    }



    public Object call (SoapObject method) throws IOException {

        requestEnvelope.setBody (method);
        call ();

        if (responseEnvelope.getBody () instanceof SoapFault)
            throw((SoapFault)responseEnvelope.getBody ());

        return responseEnvelope.getResult ();
    }


}













