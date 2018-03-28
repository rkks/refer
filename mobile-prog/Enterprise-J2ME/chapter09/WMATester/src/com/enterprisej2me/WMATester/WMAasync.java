package com.enterprisej2me.WMATester;

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import javax.microedition.io.*;
import javax.wireless.messaging.*;
 
public class WMAasync extends MIDlet
              implements CommandListener, MessageListener {
  private Display display;
  private TextField textMesg;
  private TextField dest;
  private Command doneCommand;
  private Command send1Command;
  private Command send2Command;
  private String serverPort;
  private MessageConnection sconn;

  public WMAasync () throws Exception {
    display = Display.getDisplay(this);
 
    doneCommand = new Command("DONE",
                              Command.SCREEN, 1);
    send1Command  = new Command("SEND1",
                              Command.SCREEN, 1);
    send2Command  = new Command("SEND2",
                              Command.SCREEN, 1);
 
    serverPort = getAppProperty("serverPort");
  }

  public void startApp() {
    try {
      displayBlankForm ();
      sconn = (MessageConnection) 
        Connector.open("sms://:" + serverPort);
      sconn.setMessageListener(this);
    } catch (Exception e) {
      System.out.print("Error in start");
      e.printStackTrace();
    }
  }
 
  public void pauseApp() {
    try {
      sconn.close();
    } catch (Exception e) {
      System.out.print("Error in pause");
      e.printStackTrace();
    }
  }
 
  public void destroyApp(boolean unconditional) {
    try {
      sconn.close();
    } catch (Exception e) {
      System.out.print("Error in pause");
      e.printStackTrace();
    }
  }

  public void notifyIncomingMessage(MessageConnection c) {
    // 'c' is sconn. No need to pass into 
    // the handling thread
    new Thread(new SMSHandler()).start();
    return;
  }

  // There are some potentially blocking I/O
  // operations in this callback function.
  // In real world applications, you probably
  // want to move them to a separate thread.

  public void commandAction(Command command,
                            Displayable screen) {
    if (command == doneCommand) {
      destroyApp(false);
      notifyDestroyed();
    } else if (command == send1Command) {
      try {
        String addr = "sms://+" + dest.getString(); 
        MessageConnection conn = 
          (MessageConnection) Connector.open(addr); 
        TextMessage msg = 
           (TextMessage) conn.newMessage(
           MessageConnection.TEXT_MESSAGE); 
        msg.setPayloadText( textMesg.getString() ); 
        conn.send(msg); 
        conn.close();
        displayBlankForm ();
      } catch (Exception e) {   
        System.out.println("Error in sending");
        e.printStackTrace ();
      }
    } else if (command == send2Command) {
      try {
        String addr = "sms://+" + dest.getString();
        TextMessage msg =
           (TextMessage) sconn.newMessage(
           MessageConnection.TEXT_MESSAGE);
        msg.setAddress ( addr );
        msg.setPayloadText( textMesg.getString() );
        sconn.send(msg);
        displayBlankForm ();
      } catch (Exception e) {
        System.out.println("Error in sending");
        e.printStackTrace ();
      }
    }
  }

  private void displayBlankForm () throws Exception {
    Form form = new Form ("WMATester");
    textMesg = new TextField("Message", "",
                           100, TextField.ANY);
    dest = new TextField("Phone No.", "",
                       20, TextField.ANY);
    form.append( dest );
    form.append( textMesg );
    form.addCommand(doneCommand);
    form.addCommand(send1Command);
    form.addCommand(send2Command);
    form.setCommandListener(
                   (CommandListener) this);
    display.setCurrent(form);
  }

  class SMSHandler implements Runnable {
    public void run () {
      try {
        Message msg = sconn.receive();
        if (msg instanceof TextMessage) { 
          TextMessage tmsg = (TextMessage) msg; 
          String msgText = tmsg.getPayloadText();

          // Construct the return message
          TextMessage rmsg = 
             (TextMessage) sconn.newMessage(
             MessageConnection.TEXT_MESSAGE);
          rmsg.setAddress ( tmsg.getAddress() );
          rmsg.setPayloadText( "Message " +
                               msgText + 
                               " is received" );
          sconn.send(rmsg);

          Alert alert = new Alert ("Received", msgText, 
                             null, AlertType.ERROR);
          alert.setTimeout(Alert.FOREVER);
          display.setCurrent( alert );
        } else {
          throw new Exception("Received is not a text mesg");
        }
      } catch (Exception e) {
        System.out.println("Error in server receiving");
        e.printStackTrace ();
      }
    }
  }

}
