package com.enterprisej2me.MapPoint.Client.midp;

import java.util.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import org.ksoap.*;
import org.ksoap.transport.*;

public class MIDPDirections extends MIDlet implements CommandListener {

  Display display;
  Command fromNext, toNext, cancel, done, exit;
  TextField fromStreet, fromCity, fromState, fromZip;
  TextField toStreet, toCity, toState, toZip;
  String endPointURL;

  public MIDPDirections () {

    endPointURL = "http://128.83.129.226:8080/axis/services/MapPoint";
    display = Display.getDisplay(this);

    fromNext = new Command("NEXT", Command.SCREEN, 2);
    toNext = new Command("NEXT", Command.SCREEN, 2);
    cancel = new Command("CANCEL", Command.SCREEN, 2);
    done = new Command("DONE", Command.SCREEN, 2);
    exit = new Command("EXIT", Command.SCREEN, 2);

    fromStreet = new TextField("Street", "", 20, TextField.ANY);
    fromCity = new TextField("City", "", 20, TextField.ANY);
    fromState = new TextField("State", "", 10, TextField.ANY);
    fromZip = new TextField("Zip", "", 10, TextField.NUMERIC);

    toStreet = new TextField("Street", "", 20, TextField.ANY);
    toCity = new TextField("City", "", 20, TextField.ANY);
    toState = new TextField("State", "", 10, TextField.ANY);
    toZip = new TextField("Zip", "", 10, TextField.NUMERIC);
  }

  public void startApp() {
    fromScreen ();
  }

  public void pauseApp() {
    // do nothing
  }

  public void destroyApp(boolean unconditional) {
    // do nothing
  }

  public void commandAction(Command command, Displayable screen) {
    if (command == exit) {
      destroyApp(false);
      notifyDestroyed();
    } else if ( command == done || command == cancel ) {
      startApp ();
    } else if ( command == fromNext ) {
      toScreen ();
    } else if ( command == toNext ) {
      directionScreen ();
    }
  }

  public void fromScreen () {
    Form form = new Form ("From");
    form.append(fromStreet);
    form.append(fromCity);
    form.append(fromState);
    form.append(fromZip);
    form.addCommand(fromNext);
    form.addCommand(cancel);
    form.setCommandListener( (CommandListener) this);
    display.setCurrent(form);
  }

  public void toScreen () {
    Form form = new Form ("To");
    form.append(toStreet);
    form.append(toCity);
    form.append(toState);
    form.append(toZip);
    form.addCommand(toNext);
    form.addCommand(cancel);
    form.setCommandListener( (CommandListener) this);
    display.setCurrent(form);
  }

  public void directionScreen () {
    Vector v = getDirections ();
    Form form = new Form ("Directions");
    for (int i = 0; i < v.size(); i++) {
      form.append((String) v.elementAt(i) + "\n");
    }
    form.addCommand(done);
    form.addCommand(exit);
    form.setCommandListener( (CommandListener) this);
    display.setCurrent(form);
  }


  private Vector getDirections () {
    Vector v = null;
    try {
      SoapObject method = new SoapObject("", "getDirections");
      method.addProperty("in0", fromStreet.getString());
      method.addProperty("in1", fromCity.getString());
      method.addProperty("in2", fromState.getString());
      method.addProperty("in3", fromZip.getString());
      method.addProperty("in4", toStreet.getString());
      method.addProperty("in5", toCity.getString());
      method.addProperty("in6", toState.getString());
      method.addProperty("in7", toZip.getString());
      HttpTransport rpc = new HttpTransport(endPointURL, "\"\"");
      v = (Vector) rpc.call (method);
    } catch (Exception e) {
      e.printStackTrace();
    }
    return v;
  }

}
