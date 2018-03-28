package com.enterprisej2me.MailSample;
 
import java.io.*;
import java.util.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import de.trantor.mail.*;
 
public class SendDemo extends MIDlet implements CommandListener {
 
  private Display display = Display.getDisplay(this);
  private TextField recipient;
  private TextField subject;
  private TextField body;
  private Command sendCommand;
  private Command doneCommand;
  private Command exitCommand;

  private String smtpHost;
  private String smtpUser;
  private String smtpPass;
  private String address;
  private String hostname;
  private boolean smtpAuth = true;

  public SendDemo () {
    smtpHost = getAppProperty("smtpHost");
    smtpUser = getAppProperty("smtpUser");
    smtpPass = getAppProperty("smtpPass");
    address = getAppProperty("address");
    hostname = getAppProperty("hostname");

    sendCommand = new Command("Send", Command.SCREEN, 1);
    doneCommand = new Command("Done", Command.SCREEN, 1);
    exitCommand = new Command("Exit", Command.SCREEN, 1);
  }

  protected void startApp() {
    recipient = new TextField("To:", "", 128, TextField.EMAILADDR);
    subject = new TextField("Subject:", "", 128, TextField.ANY);
    body = new TextField("Body:", "", 8192, TextField.ANY);

    Form form = new Form("Send a message");
    form.append( recipient );
    form.append( subject );
    form.append( body );
    form.addCommand( sendCommand );
    form.addCommand( exitCommand );
    form.setCommandListener(this);
 
    display.setCurrent(form);
  }
 
  protected void destroyApp(boolean unconditional) {
  }
 
  protected void pauseApp() {
  }
 
  public void commandAction (Command c, Displayable d) {
    if ( c == sendCommand ) {
      // I put all network code here for clarity.
      // In a real world app, you need to put them
      // in a separate thread.
      Message message = 
        new Message(address, recipient.getString(), subject.getString());
      message.addBodyLine( body.getString() );
 
      SmtpClient smtpClient = null;
      // use proxy
      // smtpClient = new SmtpClient(
      // new de.trantor.mail.http.ConnectionImpl(
      // httpHost, 8080), hostname);
      smtpClient = new SmtpClient(hostname);
 
      Form form;
      try {
        if (smtpAuth) {
          smtpClient.open(smtpHost, 0, false, 
                          smtpUser, smtpPass);
        } else {
          smtpClient.open(smtpHost);
        }
        smtpClient.sendMessage(message);
        form = new Form ("Success");
        form.append("Message sent");
        form.addCommand(doneCommand);
        form.setCommandListener(this);
      } catch (Exception e) {
        e.printStackTrace();
        form = new Form ("Failed");
        form.append("Sorry, sending failed");
        form.addCommand(doneCommand);
        form.setCommandListener(this);
      } finally {
        try {
          smtpClient.close();
        } catch (Exception e) {
          e.printStackTrace();
        }
      }
      display.setCurrent(form);

    } else if ( c == doneCommand ) { 
      startApp();

    } else if ( c == exitCommand ) { 
      destroyApp(false);
      notifyDestroyed();
    }
  }

}
