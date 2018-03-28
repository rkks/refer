package com.enterprisej2me.MailSample;

import java.io.*;
import java.util.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import de.trantor.mail.*;

public class InboxDemo extends MIDlet implements CommandListener {
 
  private Display display = Display.getDisplay(this);
  private Command doneCommand;
  private Command exitCommand;
  private Command inboxCommand;
  private Command delCommand;
  private Command readCommand;
  private List msgList;

  private String pop3Host;
  private String pop3User;
  private String pop3Pass;
  private boolean imap = false;

  private InboxClient inbox;

  private Vector msgNumbers = new Vector();
 
  public InboxDemo() {
    msgList = null;
    inbox = null;

    pop3Host = getAppProperty("pop3Host");
    pop3User = getAppProperty("pop3User");
    pop3Pass = getAppProperty("pop3Pass");

    doneCommand = new Command("DONE", Command.CANCEL, 1);
    exitCommand = new Command("EXIT", Command.CANCEL, 1);
    inboxCommand = new Command("INBOX", Command.SCREEN, 1);
    delCommand = new Command("DELETE", Command.SCREEN, 1);
    readCommand = new Command("READ", Command.SCREEN, 1);
  }

  protected void startApp() throws MIDletStateChangeException {
    Form form = new Form ("Welcome");
    form.append("Click INBOX to check email");
    form.addCommand(inboxCommand);
    form.addCommand(exitCommand);
    form.setCommandListener( (CommandListener) this);
    display.setCurrent(form);
  }
 
  protected void destroyApp(boolean unconditional) {
    try {
      if ( inbox != null ) {
        inbox.close();
      }
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
 
  protected void pauseApp() {
  }

  public void commandAction (Command c, Displayable d) {
    try {
      // For clarity, I put all networking here. In real world app,
      // you need to run them in a separate thread.
      if (c == inboxCommand) {
        if ( msgList == null ) {
          if (imap) {
            // If you use a proxy
            // inbox = new ImapClient(
            // new de.trantor.mail.http.ConnectionImpl(
            // httpHost, 8080));
            inbox = new ImapClient();
          } else {
            // If you use a proxy
            // inbox = new Pop3Client(
            // new de.trantor.mail.http.ConnectionImpl(
            // httpHost, 8080));
            inbox = new Pop3Client();
          }
          inbox.open(pop3Host, 0, false, pop3User, pop3Pass);
          msgList = getMessageList();
          msgList.addCommand( readCommand );
          msgList.addCommand( inboxCommand );
          msgList.addCommand( delCommand );
          msgList.addCommand( doneCommand );
          msgList.setCommandListener( (CommandListener) this);
        }
        display.setCurrent(msgList);

      } else if (c == readCommand || 
                 c == List.SELECT_COMMAND) {
        int num = msgList.getSelectedIndex();
        if (num == -1) return;
        Message message = inbox.getMessage(
          ((Integer)msgNumbers.elementAt(num)).intValue());

        Form readScreen = new Form("Message details");
        readScreen.append(
          new StringItem("From:", 
            Message.getMachineAddress(
              message.getHeaderValue("From", 
                                     "No sender"))));
        readScreen.append(
          new StringItem("Date:", 
            message.getHeaderValue("Date", "No date")));
        readScreen.append(
          new StringItem("Subject:", 
            message.getHeaderValue("Subject", "No subject")));
 
        MimeDecoder mime = new MimeDecoder(message);
        addPartToScreen(mime, readScreen);

        readScreen.addCommand( inboxCommand );
        readScreen.addCommand( doneCommand );
        readScreen.setCommandListener( (CommandListener) this);
        display.setCurrent(readScreen);

      } else if (c == delCommand) {
        int num = msgList.getSelectedIndex();
        if (num == -1) return;
        inbox.removeMessage(
          ((Integer)msgNumbers.elementAt(num)).intValue());
        msgList.delete(msgList.getSelectedIndex());
        msgNumbers.removeElementAt(num);
        display.setCurrent(msgList);

      } else if (c == doneCommand) {
        startApp ();

      } else if (c == exitCommand) {
        destroyApp(false);
        notifyDestroyed();
      }

    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  private List getMessageList() throws MailException, IOException {
    List result = new List("Inbox", Choice.IMPLICIT);
    int count = inbox.getMessageCount();
    for (int i = 0; i < count; i++) {
      String uid = inbox.getUniqueId(i);
      int size = inbox.getSize(i);
System.out.println("[INFO] Message " + i + " has ID \"" + uid + "\" and is " + size + " bytes.");
      Message message = inbox.getHeaders(i);
      result.append(
        message.getHeaderValue("Subject", "No subject")
        + " ("
        + Message.getMachineAddress(
            message.getHeaderValue("From", "No sender"))
        + ")", null);
      // msgNumbers.insertElementAt(new Integer(i), 0);
      msgNumbers.addElement(new Integer(i));
    }
    return result;
  }

  private void addPartToScreen(MimeDecoder mime, Form screen) {
    if (mime.getPartCount() == 0) {
      if ("image/png".equals(mime.getType())) {
        byte[] bytes = mime.getBodyBytes();
        screen.append(Image.createImage(bytes, 0, bytes.length));
      } else if ((mime.getType() == null) || 
                 ("text/plain".equals(mime.getType()))) {
        String s = "";
        for (int i = 0; i < mime.getBodyLineCount(); i++) {
          s = s + "\n" + mime.getBodyLine(i);
        }
        screen.append(s);
      } else {
        screen.append("\n[Unable to display \"" + 
                      mime.getType() + "\" part.]");
      }
    } else {
      for (int p = 0; p < mime.getPartCount(); p++) {
        addPartToScreen(mime.getPart(p), screen);
      }
    }
  }

}
