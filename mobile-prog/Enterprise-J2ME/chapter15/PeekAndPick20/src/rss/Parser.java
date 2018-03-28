package rss;

import java.io.*;
import java.util.Vector;
import javax.microedition.io.*;

/**
 * <code>Parser</code> is an abstract superclass for objects that
 * parse RSS and/or RDF files. Subclasses will be specific
 * to a certain XML parser. This class defines the
 * high-level behavior with which applications can interact.
 *
 * @author Jonathan Knudsen
 */
public abstract class Parser {
  protected Vector mParserListeners;
  
  /**
   * Creates a new Parser object.
   */
  public Parser() {
    mParserListeners = new Vector();
  }
  
  /**
   * Adds a new listener object.
   */
  public void addParserListener(ParserListener listener) {
    mParserListeners.addElement(listener);
  }
  
  /**
   * Subclasses can call this method to call the <code>firstItem()</code>
   * method on all registered listeners.
   */
  protected void fireFirstItem() {
    for (int i = 0; i < mParserListeners.size(); i++) {
      ParserListener listener =
          (ParserListener)mParserListeners.elementAt(i);
      listener.firstItem();
    }
  }
  
  /**
   * Subclasses can call this method to call the <code>itemParsed()</code>
   * method on all registered listeners.
   */
  protected void fireItemParsed(String title, String link,
      String description) {
    for (int i = 0; i < mParserListeners.size(); i++) {
      ParserListener listener =
          (ParserListener)mParserListeners.elementAt(i);
      listener.itemParsed(title, link, description);
    }
  }
  
  /**
   * Subclasses can call this method to call the <code>finished()</code>
   * method on all registered listeners.
   */
  protected void fireFinished() {
    for (int i = 0; i < mParserListeners.size(); i++) {
      ParserListener listener =
          (ParserListener)mParserListeners.elementAt(i);
      listener.finished();
    }
  }
  
  /**
   * Subclasses must implement this method to parse a
   * document from the given <code>InputStream</code>.
   */
  public abstract void parse(InputStream in) throws IOException;

  /**
   * Subclasses should implement this method to cancel
   * parsing.
   */
  public abstract void cancel();
}

