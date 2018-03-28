import java.io.*;
import java.net.*;
import org.apache.axis.*;
import org.apache.axis.handlers.*;

/**
 * SimpleHTTPSender is an HTTP transport handler for the Axis library, providing
 * basic HTTP communication with the server.  This class is needed in order to use
 * MapPoint .NET, because Axis does not yet support digest authentication.  The
 * CommonsHTTPSender almost works, but it currently fails with large POST requests,
 * such as those that are sent when calling GetRouteMap with a very long route.
 * These problems might be fixed in the future, eliminating the need for this class.
 *
 * You must configure Axis to use this class.  The easiest way is to provide a file
 * named "client-config.wsdd" in a directory on the classpath, containing these lines:
 *
 *   <?xml version="1.0" encoding="UTF-8"?>
 *   <deployment name="defaultClientConfig"
 *           xmlns="http://xml.apache.org/axis/wsdd/"
 *           xmlns:java="http://xml.apache.org/axis/wsdd/providers/java">
 *     <transport name="http" pivot="java:SimpleHTTPSender"/>
 *     <transport name="local" pivot="java:org.apache.axis.transport.local.LocalSender"/>
 *   </deployment>
 */
public class SimpleHTTPSender extends BasicHandler {

  /**
   * SimpleAuthenticator provides the user name and password to an URLConnection.
   */
  private static class SimpleAuthenticator extends Authenticator {
    private PasswordAuthentication m_authentication;

    SimpleAuthenticator(String userName, String password) {
      m_authentication = new PasswordAuthentication(userName, password.toCharArray());
    }

    protected PasswordAuthentication getPasswordAuthentication() {
      return m_authentication;
    }
  }


  /**
   * Invoke a remote call using an URLConnection.  This method is called by Axis.
   */
  public void invoke(MessageContext messageContext)
  throws AxisFault {
    try {
      String userName = messageContext.getUsername();
      String password = messageContext.getPassword();
      if (userName != null && password != null) {
        Authenticator.setDefault(new SimpleAuthenticator(userName, password));
      }

      URL url = new URL(messageContext.getStrProp(MessageContext.TRANS_URL));
      URLConnection conn = url.openConnection();
      writeToConnection(conn, messageContext);
      readFromConnection(conn, messageContext);
    }
    catch (Exception e) {
      throw AxisFault.makeFault(e);
    }
    finally {
      Authenticator.setDefault(null);
    }
  }


  /**
   * Write the SOAP request message to an URLConnection.
   */
  private void writeToConnection(URLConnection conn, MessageContext messageContext)
  throws Exception {
    conn.setDoOutput(true);
    Message request = messageContext.getRequestMessage();
    String contentType = request.getContentType(messageContext.getSOAPConstants());
    conn.setRequestProperty("Content-Type", contentType);
    if (messageContext.useSOAPAction()) {
      conn.setRequestProperty("SOAPAction", messageContext.getSOAPActionURI());
    }
    OutputStream out = new BufferedOutputStream(conn.getOutputStream(), 8192);
    request.writeTo(out);
    out.flush();
  }


  /**
   * Read the SOAP response message from an URLConnection.
   */
  private void readFromConnection(URLConnection conn, MessageContext messageContext)
  throws Exception {
    String contentType = conn.getContentType();
    String contentLocation = conn.getHeaderField("Content-Location");

    InputStream in = ((HttpURLConnection) conn).getErrorStream();
    if (in == null) {
      in = conn.getInputStream();
    }
    in = new BufferedInputStream(in, 8192);
    Message response = new Message(in, false, contentType, contentLocation);
    response.setMessageType(Message.RESPONSE);
    messageContext.setResponseMessage(response);
  }
}