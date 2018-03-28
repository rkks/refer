package rss;

/**
 * This interface is implemented by objects that listen for
 * events from a <code>Parser</code> subclass.
 *
 * @author Jonathan Knudsen
 */
public interface ParserListener {
  /**
   * This method is called when the first item is parsed.
   */
  public void firstItem();
  /**
   * This item is called each time an item is parsed.
   */
  public void itemParsed(String title, String link, String description);
  /**
   * This method is called when parsing is complete.
   */
  public void finished();
}

