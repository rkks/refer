package rss;

import java.io.*;
import javax.microedition.lcdui.*;

import org.kxml.*;
import org.kxml.parser.*;

/**
 * This class parses RSS 0.91 and RDF documents. It provides
 * events to registered ParserListeners.
 *
 * @author Jonathan Knudsen
 */
public class kXML12Parser
    extends Parser {
  private boolean mCancel;
  
  public kXML12Parser() {}
  
  public void parse(InputStream in) throws IOException {
    mCancel = false;
    
    Reader reader = new InputStreamReader(in);
    XmlParser parser = new XmlParser(reader);
    ParseEvent pe = null;

    parser.skip();
    pe = parser.read();
    String root = pe.getName();
    if (root.equals("rss")) {
      parser.skip();
      parser.read(Xml.START_TAG, null, "channel");
    }
    
    boolean trucking = true;
    boolean first = true;
    while (trucking && mCancel == false) {
      pe = parser.read();
      if (pe.getType() == Xml.START_TAG) {
        String name = pe.getName();
        if (name.equals("item")) {
          String title = null, link = null, description = null;
          while ((pe.getType() != Xml.END_TAG) ||
              (pe.getName().equals(name) == false)) {
            pe = parser.read();
            if (pe.getType() == Xml.START_TAG &&
                pe.getName().equals("title")) {
              pe = parser.read();
              title = pe.getText();
            }
            else if (pe.getType() == Xml.START_TAG &&
                pe.getName().equals("link")) {
              pe = parser.read();
              link = pe.getText();
            }
            else if (pe.getType() == Xml.START_TAG &&
                pe.getName().equals("description")) {
              pe = parser.read();
              description = pe.getText();
            }
          }
          if (first) {
            if (mCancel == false) fireFirstItem();
            first = false;
          }
          if (mCancel == false) fireItemParsed(title, link, description);
        }
        else {
          while ((pe.getType() != Xml.END_TAG) ||
              (pe.getName().equals(name) == false))
            pe = parser.read();
        }
      }
      if (pe.getType() == Xml.END_TAG && pe.getName().equals(root))
        trucking = false;
    }
    if (mCancel == false) fireFinished();
    mCancel = false;
  }
  
  /**
   * Cancels parsing.
   */
  public void cancel() {
    mCancel = true;
  }
}

