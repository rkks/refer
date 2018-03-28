import rss.*;

import java.io.*;
import java.util.Vector;
import javax.microedition.io.*;

import util.*;

/**
 * <code>ParseTask</code> is a <code>WorkerTask</code> that
 * can read an RSS or RDF feed. It uses an implementation
 * of <code>rss.Parser</code> to do the heavy lifting.
 *
 * @see rss.Parser
 * @author Jonathan Knudsen
 */
public class ParseTask
    implements WorkerTask {
  private Parser mParser;
  private Feed mFeed;
  private Vector mParameters;
  private HttpConnection mHttpConnection;
  private InputStream mIn;
  private boolean mCancel;
  
  /**
   * Creates a new <code>ParseTask</code>.
   *
   * @param parser the parser object to use
   * @param feed the <code>Feed</code> that will be parsed
   * @param parameters a list of <code>Parameter</code>s
   *     that may contain user input.
   *     Can be <code>null</code>.
   */
  public ParseTask(Parser parser, Feed feed,
      Vector parameters) {
    mParser = parser;
    mFeed = feed;
    mParameters = parameters;
    mCancel = false;
  }
  
  /**
   * This method connects to a feed and parses it. Events
   * are fired off by the <code>Parser</code> to
   * a registered listener.
   */
  public void run() throws IOException {
    mCancel = false;
    
    // Set up the network connection.
    mHttpConnection = null;
    mIn = null;
    
    String url = mFeed.getURL(mParameters);
    try {
      mHttpConnection = (HttpConnection)Connector.open(url);
      if (mCancel) return;
      mIn = mHttpConnection.openInputStream();
      if (mCancel) return;
      // Call the parser.
      mParser.parse(mIn);
    }
    finally {
      if (mIn != null) mIn.close();
      if (mHttpConnection != null) mHttpConnection.close();
      mIn = null;
      mHttpConnection = null;
      mCancel = false;
    }
  }
  
  /**
   * Cancels the current parsing.
   */
  public void cancel() {
    if (mHttpConnection == null) return;
    mCancel = true;
    if (mParser != null) mParser.cancel();
    try {
      if (mIn != null) mIn.close();
      if (mHttpConnection != null) mHttpConnection.close();
    }
    catch (IOException ioe) {}
    mIn = null;
    mHttpConnection = null;
  }
}
