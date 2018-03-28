package com.enterprisej2me.iFeedBack.midp;

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import com.enterprisej2me.iFeedBack.midp.MVC.*;
import com.enterprisej2me.iFeedBack.midp.Task.*;

public class Start extends MIDlet implements CommandListener {  

  private Command exitCommand;
  private Command goCommand;
  private Display display;

  // Pre-set suggestion values in JAD file
  public static String SurveyNicknameSugg;
  public static String SurveyServerURLSugg;
  public static String UsernameSugg;
  public static String PasswordSugg;

  public Start () throws Exception { 
    display = Display.getDisplay(this);
    MVCComponent.display = display;
    exitCommand = new Command("EXIT", Command.SCREEN, 2);
    goCommand = new Command("GO", Command.SCREEN, 1);
    SurveyNicknameSugg = getAppProperty("SurveyNicknameSugg");
    SurveyServerURLSugg = getAppProperty("SurveyServerURLSugg");
    UsernameSugg = getAppProperty("UsernameSugg");
    PasswordSugg = getAppProperty("PasswordSugg");
  }

  public void startApp() {
    try {
      // Initialize any exisitng security info
      // that might be stored in RMS stores.
      //
      // Other methods might use static secuirty info
      // without init UpdateToken object first.
      UpdateToken.setEndPointURL( getAppProperty("SSOEndPointURL") );
      (new UpdateToken()).prepareScreen();
    } catch (Exception e) {
      e.printStackTrace();
    }

    SplashScreen splash = new SplashScreen ();
    splash.addCommand(exitCommand);
    splash.addCommand(goCommand);
    splash.setCommandListener( (CommandListener) this);
    display.setCurrent(splash);
  }
 
  public void pauseApp() {
  }
 
  public void destroyApp(boolean unconditional) {
  }

  public void commandAction(Command command, Displayable screen) {
    if (command == exitCommand) {
      destroyApp(false);
      notifyDestroyed();

    } else if (command == goCommand) { 
      // MainMenu need to access life cycle and MIDlet methods
      MainMenu.m = this;
      (new MainMenu()).showScreen();
    }
  }
}
