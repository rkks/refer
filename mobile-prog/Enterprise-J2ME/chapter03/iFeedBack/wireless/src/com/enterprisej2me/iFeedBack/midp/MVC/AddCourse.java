package com.enterprisej2me.iFeedBack.midp.MVC;

import java.io.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import com.enterprisej2me.iFeedBack.midp.Task.*;
import com.enterprisej2me.iFeedBack.midp.*;

public class AddCourse extends MVCComponent {  

  private static Displayable screen = null;
  private static Command backCommand;
  private static Command addCommand;
  private static TextField nicknameField;
  private static TextField endpointURLField;

  // Model parameters
  private static String nickname = null;
  private static String endpointURL = null;
  // private static RecordStore courseStore;

  public static void setNickname (String s) {
    nickname = s;
  }

  public static String getNickname () {
    return nickname;
  }

  public static void setEndpointURL (String s) {
    endpointURL = s;
  }

  public static String getEndpointURL () {
    return endpointURL;
  }


  public AddCourse () { 
  }

  public Displayable getScreen() {
    return screen;
  }

  public void commandAction(Command c, Displayable s) {
    try {
      if (c == backCommand) {
        (new MainMenu()).showScreen();
      } else if (c == addCommand) {
        nickname = nicknameField.getString ();
        endpointURL = endpointURLField.getString ();

        if ( "".equals(nickname.trim()) ||
             "".equals(endpointURL.trim()) ) {
          Alert a = new Alert("Empty field");
          a.setString("Please fill out all fields");
          a.setTimeout(Alert.FOREVER);
          display.setCurrent(a);
          return;
        }

        ChooseCourse.addPair(nickname, endpointURL);
        (new ChooseCourse()).showScreen();
      }
    } catch (Exception e) {
      e.printStackTrace();
      Alert a = new Alert("Error in add course");
      a.setTimeout(Alert.FOREVER);
      display.setCurrent(a);
    }
  }

  protected void initModel() throws Exception {
    if (nickname == null) 
      nickname = Start.SurveyNicknameSugg;
    if (endpointURL == null) 
      endpointURL = Start.SurveyServerURLSugg;
  }

  protected void createView() throws Exception {
    backCommand = new Command("MAIN", Command.SCREEN, 2);
    addCommand = new Command("ADD", Command.SCREEN, 1);
    nicknameField = new TextField("Nickname:", nickname,
                                  40, TextField.ANY);
    endpointURLField = new TextField("End Point URL:", endpointURL,
                                  120, TextField.ANY);

    screen = new Form("Add a new course");
    ((Form) screen).append( nicknameField );
    ((Form) screen).append( endpointURLField );
    screen.addCommand( backCommand );
    screen.addCommand( addCommand );
  }

  protected void updateView() throws Exception {
    nicknameField.setString(nickname);
    endpointURLField.setString(endpointURL);
  }

}
