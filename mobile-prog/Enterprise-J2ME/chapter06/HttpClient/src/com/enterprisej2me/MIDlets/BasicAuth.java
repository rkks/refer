package com.enterprisej2me.MIDlets;
 
import javax.microedition.midlet.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
import com.enterprisej2me.HttpConnections.*;
 
public class BasicAuth extends MIDlet implements CommandListener {

  private Display display;
  private Command exitCommand;

  public BasicAuth () throws Exception {
    display = Display.getDisplay(this);
    exitCommand = new Command("EXIT", Command.CANCEL, 1);
  }

  public void startApp() {
    Form form = new Form("Basic Auth client");

    try {
      String username = getAppProperty("BasicAuthUser");
      String password = getAppProperty("BasicAuthPass");
      String url = getAppProperty("BasicAuthURL");

      HttpClient client = new HttpClient ();
      Handler h = new BasicAuthHandler(username, password);
      client.addHandler( h );

      client.setUrl( url );
      client.setRequestMethod( HttpConnection.GET );
      byte [] result = client.query(null);
      form.append( new String(result) );
    } catch (Exception e) {
     e.printStackTrace();
     form.append("Sorry, there is an error fetching");
    }

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
    } else {
      // Do nothing.
    }
  }
 
}
