package com.enterprisej2me.MIDlets;
 
import javax.microedition.midlet.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
import com.enterprisej2me.HttpConnections.*;
 
public class AuthSession extends MIDlet implements CommandListener {

  private Display display;
  private Command exitCommand;
  private Command countCommand;
  private HttpClient client;

  public AuthSession () throws Exception {
    display = Display.getDisplay(this);
    countCommand = new Command("COUNT", Command.CANCEL, 1);
    exitCommand = new Command("EXIT", Command.CANCEL, 1);
  }

  public void startApp() {
    Form form = new Form("Session client");

    try {
      String url = getAppProperty("AuthSessionURL");
      String username = getAppProperty("AuthSessionUser");
      String password = getAppProperty("AuthSessionPass");

      client = new HttpClient ();
      Handler h1 = new CookieHandler();
      Handler h2 = new BasicAuthHandler(username, password);
      client.addHandler( h1 );
      client.addHandler( h2 );
      client.setUrl( url );
      client.setRequestMethod( HttpConnection.GET );

    } catch (Exception e) {
     e.printStackTrace();
     form.append("Sorry, there is an error fetching");
    }

    form.addCommand(countCommand);
    form.addCommand(exitCommand);
 
    // The CommandListener is this MIDlet itself.
    form.setCommandListener(this);
 
    // Display the form on the LCD screen.
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

    } else if (command == countCommand) {

      Worker t = new Worker ();
      t.setListener (this);
      t.start();

    } else {
      // Do nothing.
    }
  }

  class Worker extends Thread {

    private CommandListener listener;

    public void setListener (CommandListener cl) {
      listener = cl;
    }

    public void run () {

      Form form = new Form("Session client");
      String resultStr;
      try {
        byte [] result = client.query(null);
        resultStr = new String(result);
      } catch (Exception e) {
        resultStr = "fetch failed";
      }
      form.append( resultStr );
      form.addCommand(countCommand);
      form.addCommand(exitCommand);

      form.setCommandListener(listener);
      display.setCurrent(form);
    }

  }

}
