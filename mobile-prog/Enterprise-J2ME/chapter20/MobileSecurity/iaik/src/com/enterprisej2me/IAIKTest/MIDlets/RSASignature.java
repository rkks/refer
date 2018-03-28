/*
 * RSASignature.java
 *
 * Created on November 21, 2002, 3:31 AM
 */

package com.enterprisej2me.IAIKTest.MIDlets;

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import java.util.*;
import com.enterprisej2me.IAIKTest.utils.*;

/**
 *
 * @author Michael Yuan
 */
public class RSASignature extends MIDlet implements CommandListener {
  
  private Display display;
  private Command exitCommand;
  private Command doneCommand;
  private Command keygenCommand;
  private Command signCommand;
  private Command verifyCommand;
  private TextField textField;

  private CryptoEngine engine;
  private byte [] message;
  private byte [] signature;

  private long startTime, endTime, interval;

  /** Creates a new instance of RSASignature */
  public RSASignature() throws Exception {
    // Initialize display and command objects.
    display = Display.getDisplay(this);
    exitCommand = new Command("EXIT", Command.CANCEL, 1);
    doneCommand = new Command("DONE", Command.CANCEL, 1);
    keygenCommand = new Command("GenKeys", Command.SCREEN, 1);
    signCommand = new Command("Sign", Command.SCREEN, 1);
    verifyCommand = new Command("Verify", Command.SCREEN, 1);
    engine = new CryptoEngine ();
  }

  public void startApp() {
    Form form = new Form("RSA signature example");
    textField = new TextField("Message:", "",
                              80, TextField.ANY);
    form.append( textField );
 
    form.addCommand(signCommand);
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
        engine.generateRSAKeyPair ();
        endTime = (new Date ()).getTime();
        interval = endTime - startTime;

        Alert a = new Alert("success!");
        a.setString("RSA Key is generated in " 
                    + interval + " millisec");
        a.setTimeout(Alert.FOREVER);
        display.setCurrent (a);
      } catch (Exception e) {
        e.printStackTrace();
      }
      
    } else if (command == signCommand) {
      try {
        message = textField.getString().getBytes();
        startTime = (new Date ()).getTime();
        signature = engine.RSASign (message);
        endTime = (new Date ()).getTime();
        interval = endTime - startTime;

        Form form = new Form ("RSA signed");
        form.append ("RSA signature generated in " 
                     + interval + " millisec\n");
        form.append ("Signature:\n");
        form.append (new String(signature));

        form.addCommand (verifyCommand);
        form.setCommandListener ((CommandListener) this);
        display.setCurrent(form);
      } catch (Exception e) {
        e.printStackTrace();
      }
      
    } else if (command == verifyCommand) {
      try {
        startTime = (new Date ()).getTime();
        boolean valid = engine.RSAVerify (message, signature);
        endTime = (new Date ()).getTime();
        interval = endTime - startTime;

        Form form = new Form ("RSA verified");
        form.append ("RSA signature verified in " 
                     + interval + " millisec\n");
        form.append ("Result is: \n");
        if (valid) {
          form.append("success!");
        } else {
          form.append("failed!");
        }

        form.addCommand (exitCommand);
        form.addCommand (doneCommand);
        form.setCommandListener ((CommandListener) this);
        display.setCurrent(form);
      } catch (Exception e) {
        e.printStackTrace();
      }
      
    } else {
      // do nothing
    }
  }
}
