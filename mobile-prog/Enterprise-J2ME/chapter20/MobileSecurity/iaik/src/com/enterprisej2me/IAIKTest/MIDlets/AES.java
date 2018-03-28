package com.enterprisej2me.IAIKTest.MIDlets;

import java.util.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import com.enterprisej2me.IAIKTest.utils.*;

public class AES extends MIDlet implements CommandListener {
 
  private Display display;
  private Command exitCommand;
  private Command doneCommand;
  private Command keygenCommand;
  private Command encryptCommand;
  private Command decryptCommand;
  private TextField textField;

  private CryptoEngine engine;
  private byte [] original;
  private byte [] encrypted;
  private byte [] decrypted;

  private long startTime, endTime, interval;

  public AES () throws Exception {
    // Initialize display and command objects.
    display = Display.getDisplay(this);
    exitCommand = new Command("EXIT", Command.CANCEL, 1);
    doneCommand = new Command("DONE", Command.CANCEL, 1);
    keygenCommand = new Command("GenKeys", Command.SCREEN, 1);
    encryptCommand = new Command("Encrypt", Command.SCREEN, 1);
    decryptCommand = new Command("Decrypt", Command.SCREEN, 1);
    engine = new CryptoEngine ();
  }
 
  public void startApp() {
    Form form = new Form("AES example");
    textField = new TextField("Plain Text:", "",
                              80, TextField.ANY);
    form.append( textField );
 
    form.addCommand(encryptCommand);
    form.addCommand(keygenCommand);
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
 
    } else if (command == keygenCommand) {
      try {
        startTime = (new Date ()).getTime();
        engine.generateAESKey ();
        endTime = (new Date ()).getTime();
        interval = endTime - startTime;

        Alert a = new Alert("success!");
        a.setString("AES Key is generated in " 
                    + interval + " millisec");
        a.setTimeout(Alert.FOREVER);
        display.setCurrent (a);
      } catch (Exception e) {
        e.printStackTrace();
      }
 
    } else if (command == encryptCommand) {
      try {
        original = textField.getString().getBytes();
        startTime = (new Date ()).getTime();
        encrypted = engine.AESEncrypt(original);
        endTime = (new Date ()).getTime();
        interval = endTime - startTime;
 
        Form form = new Form ("AES Encrypted");
        form.append ("AES encryption finished in " 
                     + interval + " millisec\n");
        form.append ("Result:\n");
        form.append (new String(encrypted));

        form.addCommand (decryptCommand);
        form.setCommandListener ((CommandListener) this);
        display.setCurrent(form);
      } catch (Exception e) {
        e.printStackTrace();
      }

    } else if (command == decryptCommand) {
      try {
        startTime = (new Date ()).getTime();
        decrypted = engine.AESDecrypt(encrypted);
        endTime = (new Date ()).getTime();
        interval = endTime - startTime;
 
        Form form = new Form ("AES Decrypted");
        form.append ("AES decryption finished in " 
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
