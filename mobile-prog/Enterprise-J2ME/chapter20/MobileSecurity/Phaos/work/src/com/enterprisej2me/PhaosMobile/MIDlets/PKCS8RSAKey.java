package com.enterprisej2me.PhaosMobile.MIDlets;

import java.util.*;
import java.io.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import com.enterprisej2me.PhaosMobile.utils.*;
import com.phaos.micro.crypto.pkcs1.*;

public class PKCS8RSAKey extends MIDlet implements CommandListener {
 
  private Display display;
  private Command exitCommand;
  private Command doneCommand;
  private Command encryptCommand;
  private Command decryptCommand;
  private TextField passwdField;

  private CryptoEngine engine;
  private RSAPrivateKey original, decrypted;
  private byte [] encrypted;

  private long startTime, endTime, interval;

  public PKCS8RSAKey () throws Exception {
    // Initialize display and command objects.
    display = Display.getDisplay(this);
    exitCommand = new Command("EXIT", Command.CANCEL, 1);
    doneCommand = new Command("DONE", Command.CANCEL, 1);
    encryptCommand = new Command("Encrypt", Command.SCREEN, 1);
    decryptCommand = new Command("Decrypt", Command.SCREEN, 1);
    engine = new CryptoEngine ();
  }
 
  public void startApp() {
    try {
      Class c = this.getClass(); 
      InputStream is = c.getResourceAsStream("/keys/RSAprivKey.der");
      original = new RSAPrivateKey(is);
      is.close();
    } catch (Exception e) {
      e.printStackTrace();
    }
    Form form = new Form("Key encryption example");
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
        String passwdStr = passwdField.getString();

        startTime = (new Date ()).getTime();
        encrypted = engine.PKCS8EncryptRSAKey(original, passwdStr);
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

        startTime = (new Date ()).getTime();
        decrypted = engine.PKCS8DecryptRSAKey(encrypted, passwdStr);
        endTime = (new Date ()).getTime();
        interval = endTime - startTime;
 
        Form form = new Form ("Decrypted");
        form.append ("Decryption finished in " 
                     + interval + " millisec\n");
        form.append ("Result:\n");
        if ( (original.publicExponent).equals(decrypted.publicExponent)
         &&  (original.primeP).equals(decrypted.primeP)
         &&  (original.primeQ).equals(decrypted.primeQ)
         &&  (original.primeExponentP).equals(decrypted.primeExponentP)
         &&  (original.primeExponentQ).equals(decrypted.primeExponentQ)) {
          form.append("Success! \n");
        } else {
          form.append("Failure! \n");
        }

        if ( (original.publicExponent.toString()).equals(decrypted.publicExponent.toString())
         &&  (original.primeP.toString()).equals(decrypted.primeP.toString())
         &&  (original.primeQ.toString()).equals(decrypted.primeQ.toString())
         &&  (original.primeExponentP.toString()).equals(decrypted.primeExponentP.toString())
         &&  (original.primeExponentQ.toString()).equals(decrypted.primeExponentQ.toString())) {
          form.append("Success! \n");
        } else {
          form.append("Failure! \n");
        }

/* debug
        form.append(original.publicExponent.toString() + "\n\n");
        form.append(decrypted.publicExponent.toString() + "\n\n");
        form.append(original.primeP.toString() + "\n\n");
        form.append(decrypted.primeP.toString() + "\n\n");
        form.append(original.primeQ.toString() + "\n\n");
        form.append(decrypted.primeQ.toString() + "\n\n");
        form.append(original.primeExponentP.toString() + "\n\n");
        form.append(decrypted.primeExponentP.toString() + "\n\n");
        form.append(original.primeExponentQ.toString() + "\n\n");
        form.append(decrypted.primeExponentQ.toString() + "\n\n");
*/
 
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
