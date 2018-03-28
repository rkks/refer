package com.enterprisej2me.AmazonExample;
 
import java.util.*;
import java.io.*;
import javax.microedition.midlet.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
import org.kxml2.io.*;
import org.kxml2.kdom.*;
import org.xmlpull.v1.*;
 
public class AmazonLite extends MIDlet implements CommandListener {

  Display display;
  Command pullCommand;
  Command kdomCommand;
  Command exitCommand;
  Command doneCommand;
  TextField textField;

  static String token;

  public AmazonLite () {
    display = Display.getDisplay(this);
 
    pullCommand = new Command("PULL", Command.SCREEN, 1);
    kdomCommand = new Command("kDOM", Command.SCREEN, 1);
    exitCommand = new Command("EXIT", Command.EXIT, 1);
    doneCommand = new Command("DONE", Command.CANCEL, 1);

    // Get value from the JAD file
    token = getAppProperty("AmazonToken");
  }

  public void startApp() {
    Form form = new Form("Amazon Search");
    textField = new TextField ("Keywords:", "",
                               80, TextField.ANY);
    form.append( textField );
    form.addCommand(exitCommand);
    form.addCommand(pullCommand);
    form.addCommand(kdomCommand);
    form.setCommandListener( (CommandListener) this);
    display.setCurrent(form);
  }
 
  public void pauseApp() {
  }
 
  public void destroyApp(boolean unconditional) {
  }

  public void commandAction(Command command, Displayable screen) {
    if (command == exitCommand) {
      destroyApp(false);
      notifyDestroyed();
 
    } else if ( command == doneCommand ) {
      startApp ();

    } else if ( command == pullCommand || 
                command == kdomCommand) {
      // In real production system, we should put
      // all network and parsing tasks in a seperate
      // thread. I put all here for simplicity.

      String keywords = textField.getString();
      keywords = keywords.trim();
      if ( "".equals(keywords) ) {
        Alert a = new Alert("Blank search string");
        a.setString("Please enter one or more keywords");
        a.setTimeout(Alert.FOREVER);
        display.setCurrent(a);
        return;
      }
      keywords = WSencode(keywords);

      String url = "http://xml.amazon.com/onca/xml?v=1.0"
        + "&t=webservices-20&dev-t=" + token 
        + "&KeywordSearch=" + keywords
        + "&mode=books&type=lite&page=1&f=xml";

      Vector books = new Vector ();
      try {
        HttpConnection conn = 
          (HttpConnection) Connector.open (url);
        conn.setRequestMethod(HttpConnection.GET);
        InputStream is = conn.openInputStream ();
        if ( command == pullCommand ) {
          books = getBooksViaPull( is );
        } else {
          books = getBooksViaDOM( is );
        }
        is.close();
        conn.close();
      } catch (Exception e) {
        e.printStackTrace();
      }

      Form form = new Form("Results");
      for (int i = 0; i < books.size(); i++) {
        BookDetails bd = (BookDetails) books.elementAt(i);
        form.append("\"" + bd.title + "\" ");
        form.append("By " + bd.firstAuthor + "\n");
        form.append("Amazon price " + bd.newPrice + "\n");
        form.append("Used price " + bd.usedPrice + "\n");
        form.append(bd.url + "\n\n");
      }
      form.addCommand(doneCommand);
      form.setCommandListener( (CommandListener) this);
      display.setCurrent(form);

    } else {
      // Do nothing
    }
  }

  // Get rid of excessive white spaces and replace significant
  // white spaces with %20
  String WSencode(String s) {
    StringBuffer buf = new StringBuffer ();
    int len = s.length();
    boolean blank = false;
    for (int i = 0; i < len; i++) {
      if ( s.charAt(i) == ' ' ) {
        if ( !blank ) {
          buf.append("%20");
          blank = true;
        }
      } else {
        buf.append( s.charAt(i) );
        blank = false;
      }
    }
    return buf.toString();
  }

  Vector getBooksViaDOM (InputStream is) throws Exception {
    Vector books = new Vector ();
 
    InputStreamReader reader = new InputStreamReader(is);
    KXmlParser parser = new KXmlParser();
    parser.setInput(reader);

    Document doc = new Document ();
    doc.parse (parser);

    // Use the following code to write 
    // in memory doc object to a stream
    // KXmlSerializer serializer = new KXmlSerializer ();
    // serializer.setOutput (System.out, null); 
    // doc.write (serializer);
    // serializer.flush ();

    // The <ProductInfo> element
    Element prods = doc.getRootElement();

    int numOfEntries = prods.getChildCount ();
    for (int i = 0; i < numOfEntries; i++) {
      if ( prods.isText(i) ) {
        // Text here are all insignificant white spaces.
        // We are only interested in children elements
      } else {
        // Not text, must be a <Details> element
        Element e = prods.getElement (i);
        BookDetails bd = getBookDetailsViaDOM( e );
        books.addElement( bd );
      }
    }
    return books;
  }

  BookDetails getBookDetailsViaDOM (Element e)
                                          throws Exception {
    BookDetails bd = new BookDetails ();
    // get attribute value from the <Details> start tag
    bd.url = e.getAttributeValue(null, "url");
    int numOfChildren = e.getChildCount ();
    for (int i = 0; i < numOfChildren; i++) {
      if ( e.isText(i) ) {
        // Ignore
      } else {
        Element c = e.getElement(i);
        String tagname = c.getName();
        if ( tagname.equals("ProductName") ) {
          // First child is a text node
          bd.title = c.getText(0).trim();
        }
        if ( tagname.equals("Authors") ) {
          // Goes down the tree: The second child is the
          // first <Author> element. Get the first child of
          // that element.
          bd.firstAuthor = 
	    c.getElement(1).getText(0).trim();
        }
        if ( tagname.equals("OurPrice") ) {
          // First child is a text node
          bd.newPrice = c.getText(0).trim();
        }
        if ( tagname.equals("UsedPrice") ) {
          // First child is a text node
          bd.usedPrice = c.getText(0).trim();
        }
      }
    }
    return bd;
  }

  Vector getBooksViaPull (InputStream is) throws Exception {
    Vector books = new Vector ();

    InputStreamReader reader = new InputStreamReader(is);
    KXmlParser parser = new KXmlParser();
    parser.setInput(reader);
    int eventType = parser.getEventType();
    while (eventType != parser.END_DOCUMENT) {
      // Only respond to the <Details> start tag
      if (eventType == parser.START_TAG) {
        if ( parser.getName().equals("Details") ) {
          BookDetails bd = getBookDetailsViaPull(parser);
          books.addElement( bd );
        }
      }
      eventType = parser.next();
    }
    return books;
  }

  BookDetails getBookDetailsViaPull (XmlPullParser parser) 
                                          throws Exception {
    BookDetails bd = new BookDetails ();
    // get attribute value from the <Details> start tag
    bd.url = parser.getAttributeValue(null, "url");
    int eventType = parser.next();
    while ( true ) {
      // Break out the loop at </Details> end tag
      if ( eventType == parser.END_TAG ) {
        if ( parser.getName().equals("Details") ) {
          break;
        }
      }

      if ( eventType == parser.START_TAG ) {
        String tagname = parser.getName();
        if ( tagname.equals("ProductName") ) {
          // Proceed to the enclosed Text node
          parser.next();
          bd.title = parser.getText().trim();
        }
        if ( tagname.equals("Authors") ) {
          // First <Author> start tag
          parser.next();
          // White space between tags
          parser.next();
          // Proceed to the enclosed Text node
          parser.next();
          bd.firstAuthor = parser.getText().trim();
        }
        if ( tagname.equals("OurPrice") ) {
          // Proceed to the enclosed Text node
          parser.next();
          bd.newPrice = parser.getText().trim();
        }
        if ( tagname.equals("UsedPrice") ) {
          // Proceed to the enclosed Text node
          parser.next();
          bd.usedPrice = parser.getText().trim();
        }
      }
      eventType = parser.next();
    } 
    return bd;
  }

}

class BookDetails {

  String url;
  String title;
  String firstAuthor;
  String newPrice;
  String usedPrice;

  public BookDetails () {
    url = "http://unknown";
    title = "undefined";
    firstAuthor = "unknown";
    newPrice = "unknown";
    usedPrice = "unknown";
  }

}
