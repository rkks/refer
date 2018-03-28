package com.enterprisej2me.iFeedBack.midp.MVC;

import java.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import com.enterprisej2me.iFeedBack.midp.Task.*;
import com.enterprisej2me.iFeedBack.midp.*;

public class UpdateToken extends MVCComponent {  

  private static Displayable screen = null; 
  private static Command backCommand;
  private static Command updateCommand;
  private static TextField usernameField;
  private static TextField passwordField;
  private static RecordStore securityInfoStore;

  // Model parameters
  private static String username;
  private static String password;
  private static String token;
  private static String endPointURL;

  public static String getUsername () {
    return username;
  }

  public static String getPassword () {
    return password;
  }

  public static String getToken () {
    return token;
  }

  public static void setEndPointURL (String s) {
    endPointURL = s;
  }

  public static void setAll (String u, String p, String t) 
                                         throws Exception {
    username = u;
    password = p;
    token = t;

    ByteArrayOutputStream baos = new ByteArrayOutputStream ();
    DataOutputStream dout = new DataOutputStream (baos);
    dout.writeUTF (username);
    dout.writeUTF (password);
    dout.writeUTF (token);
    dout.flush();
    byte [] newrecord = baos.toByteArray();
    dout.close();
    baos.close();
 
    securityInfoStore = RecordStore.openRecordStore("securityInfo", true);
    RecordEnumeration re =
           securityInfoStore.enumerateRecords(null, null, false);
    if ( re.hasNextElement() ) {
      securityInfoStore.deleteRecord( re.nextRecordId() );
    }
    securityInfoStore.addRecord(newrecord, 0, newrecord.length);
    re.destroy();
    securityInfoStore.closeRecordStore();
  }

  public static void clearAll () throws Exception {
    username = "";
    password = "";
    token = "-1:-1";

    securityInfoStore = RecordStore.openRecordStore("securityInfo", true);
    RecordEnumeration re = 
          securityInfoStore.enumerateRecords(null, null, false);
    while ( re.hasNextElement() ) {
      int id = re.nextRecordId ();
      securityInfoStore.deleteRecord( id );
    }
    securityInfoStore.closeRecordStore();
  }

  public UpdateToken () { 
  }

  public Displayable getScreen () {
    return screen;
  }

  public void commandAction(Command c, Displayable s) {
    try {
      if (c == backCommand) {
        (new MainMenu()).showScreen();
      } else if (c == updateCommand) {
        username = usernameField.getString();
        password = passwordField.getString();
        UpdateTokenTask t = new UpdateTokenTask(endPointURL, display);
        t.go();
      }
    } catch (Exception e) {
      Alert a = new Alert("Error in UpdateToken");
      a.setTimeout(Alert.FOREVER);
      display.setCurrent(a);
    }
  }

  protected void initModel() throws Exception {
    try {
      securityInfoStore = RecordStore.openRecordStore("securityInfo",
                                                      true);
      RecordEnumeration re = securityInfoStore.enumerateRecords(null,
                                                                null,
                                                                false);
      ByteArrayInputStream bais = new ByteArrayInputStream(
                                      re.nextRecord() );
      DataInputStream din = new DataInputStream(bais);
      username = din.readUTF();
      password = din.readUTF();
      token = din.readUTF();
      din.close();
      bais.close();
      securityInfoStore.closeRecordStore(); 
    } catch (Exception e) {
      username = Start.UsernameSugg;
      password = Start.PasswordSugg;
      token = "-1:-1";
    }
  }

  protected void createView() throws Exception {
    backCommand = new Command("MAIN", Command.SCREEN, 2);
    updateCommand = new Command("UPDATE", Command.SCREEN, 1);
    usernameField = new TextField("Username:", username,
                                  40, TextField.ANY);
    passwordField = new TextField("Password:", password,
                                  40, TextField.ANY);

    screen = new Form("Security Info");
    ((Form) screen).append( usernameField );
    ((Form) screen).append( passwordField );
    // The itemNum should be 2 for the following one
    ((Form) screen).append( "Current Token is " + token );
    screen.addCommand( backCommand );
    screen.addCommand( updateCommand );
  }

  protected void updateView() throws Exception {
    // display updated username and password
    usernameField.setString( username );
    passwordField.setString( password );
    // Deletes the old token
    ((Form) screen).delete(2);
    if ( "-1:-1".equals(token) ) {
      ((Form) screen).append("The current token is not valid");
    } else {
      ((Form) screen).append("Current Token is " + token);
    }
  }

}
