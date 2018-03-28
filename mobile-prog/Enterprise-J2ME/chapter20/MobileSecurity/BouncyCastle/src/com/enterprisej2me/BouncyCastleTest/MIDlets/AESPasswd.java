package com.enterprisej2me.BouncyCastleTest.MIDlets;

import java.util.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import com.enterprisej2me.BouncyCastleTest.utils.*;

public class AESPasswd extends MIDlet implements CommandListener {
 
  private Display display;
  private Command exitCommand;
  private Command doneCommand;
  private Command encryptCommand;
  private Command decryptCommand;
  private TextField textField;
  private TextField passwdField;

  private CryptoEngine engine;
  private byte [] original;
  private byte [] encrypted;
  private byte [] decrypted;

  private long startTime, endTime, interval;

  public AESPasswd () throws Exception {
    // Initialize display and command objects.
    display = Display.getDisplay(this);
    exitCommand = new Command("EXIT", Command.CANCEL, 1);
    doneCommand = new Command("DONE", Command.CANCEL, 1);
    encryptCommand = new Command("Encrypt", Command.SCREEN, 1);
    decryptCommand = new Command("Decrypt", Command.SCREEN, 1);
    engine = new CryptoEngine ();
  }
 
  public void startApp() {
    Form form = new Form("Password example");
    textField = new TextField("Plain Text:", "",
                              80, TextField.ANY);
    form.append( textField );
    passwdField = new TextField("Password:", "MyPassword",
                              20, TextField.ANY);
    form.append( passwdField );
 
    form.addCommand(encryptCommand);
    // The CommandListener is this MIDlet itself.
    form.setCommandListener( (CommandListener) this);
 
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
 
    } else if (command == doneCommand) {
      startApp();
 
    } else if (command == encryptCommand) {
      try {
        original = textField.getString().getBytes();
        String passwdStr = passwdField.getString();
        char [] passwd = passwdStr.toCharArray();

        startTime = (new Date ()).getTime();
        encrypted = engine.AESPasswdEncrypt(original, passwd);
        endTime = (new Date ()).getTime();
        interval = endTime - startTime;
 
        Form form = new Form ("Encrypted");
        form.append ("Password encryption finished in " 
                     + interval + " millisec\n");
        form.append ("Result:\n");
        form.append (new String(encrypted));
        passwdField = new TextField("Password:", passwdStr,
                                    20, TextField.ANY);
        form.append( passwdField );

        form.addCommand (decryptCommand);
        form.setCommandListener ((CommandListener) this);
        display.setCurrent(form);
      } catch (Exception e) {
        e.printStackTrace();
      }

    } else if (command == decryptCommand) {
      try {
        String passwdStr = passwdField.getString();
        char [] passwd = passwdStr.toCharArray();

        startTime = (new Date ()).getTime();
        decrypted = engine.AESPasswdDecrypt(encrypted, passwd);
        endTime = (new Date ()).getTime();
        interval = endTime - startTime;
 
        Form form = new Form ("Decrypted");
        form.append ("Password decryption finished in " 
                     + interval + " millisec\n");
        form.append ("Result:\n");
        form.append (new String(decrypted));
 
        form.addCommand (exitCommand);
        form.addCommand (doneCommand);
        form.setCommandListener ((CommandListener) this);
        display.setCurrent(form);
      } catch (Exception e) {
        e.printStackTrace();
      }

    } else {
      // Do nothing.
    }
  }
}
