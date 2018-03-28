import java.io.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;

import util.*;

/**
 * <code>MailTask</code> is a <code>WorkerTask</code> that
 * can contact a servlet to send e-mail on behalf of the
 * user. The protocol between this class and the server
 * is documented in <i>PeekAndPick 2.0 Design and Implementation</i>.
 *
 * @author Jonathan Knudsen
 */
public class MailTask
    implements WorkerTask {
  private Display mDisplay;
  private String mURL, mLog, mEmail, mMessage;
  
  private HttpConnection mHttpConnection;
  private OutputStream mOut;
  
  private int mResponseCode;
  private String mResponseMessage;
  
  private boolean mCancel;
  
  /**
   * Creates a new <code>MailTask</code>.
   *
   * @param url the URL of the servlet to be used
   * @param log a message for the server's log
   * @param email the destination email address
   * @param message the message itself
   */
  public MailTask(String url, String log,
      String email, String message) {
    mURL = url;
    mLog = log;
    mEmail = email;
    mMessage = message;
  }
  
  /**
   * The server sends back a response code which can
   * be retrieved (after <code>run()</code> completes)
   * using this method.
   *
   * @return the response code from the server
   */
  public int getResponseCode() { return mResponseCode; }
  
  /**
   * The server sends back a response message which can
   * be retrieved (after <code>run()</code> completes)
   * using this method.
   *
   * @return the response message from the server
   */
  public String getResponseMessage() {
    return mResponseMessage;
  }
  
  /**
   * This method connects to the server and sends the
   * message. It also collects a response code and message
   * from the server that can be retrieved using
   * <code>getResponseCode()</code> and
   * <code>getResponseMessage()</code>.
   *
   * @see #getResponseCode
   * @see #getResponseMessage
   */
  public void run() throws IOException {
    String url = mURL;
    mHttpConnection = null;
    mOut = null;
    
    try {
      if (mCancel) return;
      mHttpConnection = (HttpConnection)Connector.open(url);
      mHttpConnection.setRequestMethod(HttpConnection.POST);
      mHttpConnection.setRequestProperty("Content-Type", "text/plain");
      // No content length.
      
      if (mCancel) return;
      // Prepare the content output stream.
      mOut = mHttpConnection.openOutputStream();
      PrintStream pout = new PrintStream(mOut);
      
      // Write out the log string.
      pout.println(mLog);
      
      // Write out our destination address.
      pout.println(mEmail);
      
      if (mCancel) return;
      // Write out the message.
      pout.println(mMessage);
      
      if (mCancel) return;
      // Force the request to be written.
      InputStream in = mHttpConnection.openInputStream();
      
      if (mCancel) return;
      // Read the server response code and message.
      int length = (int)mHttpConnection.getLength();
      if (length == -1) length = 255;
      byte[] raw = new byte[length];
      in.read(raw);
      String s = new String(raw);
      String codeString = s.substring(0, 4).trim();
      mResponseCode = Integer.parseInt(codeString);
      mResponseMessage = s.substring(4).trim();
    }
    finally {
      if (mOut != null) mOut.close();
      if (mHttpConnection != null) mHttpConnection.close();
      mHttpConnection = null;
      mOut = null;
      mCancel = false;
    }
  }
  
  /**
   * Cancels a pending connection.
   */
  public void cancel() {
    if (mHttpConnection == null) return;
    mCancel = true;
    try {
      mOut.close();
      mHttpConnection.close();
    }
    catch (IOException ioe) {}
    mOut = null;
    mHttpConnection = null;
  }
}
