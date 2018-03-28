package com.enterprisej2me.iFeedBack.midp.MVC;
 
import javax.microedition.lcdui.*;

public abstract class MVCComponent implements CommandListener {

//  protected static Displayable screen = null;

  // Set from outside at beginning
  public static Display display;

  // Returns the screen object from the derived class
  public abstract Displayable getScreen();

  public Displayable prepareScreen () throws Exception {
    if ( getScreen() == null ) {
      initModel();
      createView();
    } else {
      updateView();
    }
    getScreen().setCommandListener ( (CommandListener) this );
    return getScreen ();
  }

  public void showScreen() {
    try {
      display.setCurrent( prepareScreen() );
    } catch (Exception e) {
      e.printStackTrace();
      Alert a = new Alert("Error in showing screen");
      a.setTimeout(Alert.FOREVER);
      display.setCurrent(a);
    }
  }

  // Initialize. If a data member is not backed by RMS, make sure
  // it is uninitilzed (null) before you put in values.
  protected abstract void initModel () throws Exception;

  protected abstract void createView () throws Exception;

  protected abstract void updateView () throws Exception;

  public abstract void commandAction(Command c, Displayable s);

}
