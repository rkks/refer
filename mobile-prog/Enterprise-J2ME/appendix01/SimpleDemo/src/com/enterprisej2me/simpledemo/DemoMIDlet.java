package com.enterprisej2me.simpledemo;

import java.io.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import javax.microedition.io.*;

public class DemoMIDlet extends MIDlet
                implements CommandListener {

  private Display display;
  private Command fetch;
  private Command history;
  private Command erase;
  private Command exit;
  private Command done;
  private TextField urlField;
  private RecordStore store;

  public DemoMIDlet () throws Exception {
    display = Display.getDisplay(this);
    fetch = new Command("FETCH", Command.SCREEN, 1);
    history = new Command("HISTORY", Command.SCREEN, 1);
    erase = new Command("ERASE", Command.SCREEN, 1);
    exit  = new Command("EXIT", Command.CANCEL, 1);
    done  = new Command("DONE", Command.CANCEL, 1);
    store = RecordStore.openRecordStore("DataStore", true);
  }

  public void startApp() {
    startScreen ();
  }

  public void pauseApp() {
    // Nothing to do ...
  }

  public void destroyApp(boolean unconditional) {
    try {
      store.closeRecordStore ();
    } catch (Exception exp) {
      exp.printStackTrace ();
    }

  }


  public void commandAction(Command command,
                            Displayable screen) {
    if (command == exit) {

      destroyApp(false);
      notifyDestroyed();

    } else if (command == fetch) {

      FetchWorker t = new FetchWorker ();
      t.setListener(this);
      t.start();

    } else if (command == history) {

      HistoryWorker t = new HistoryWorker ();
      t.setListener(this);
      t.start();

    } else if (command == erase) {

      try {
        store.closeRecordStore ();
        RecordStore.deleteRecordStore("DataStore");
        store = RecordStore.openRecordStore("DataStore", true);
      } catch (Exception exp) { }

      HistoryWorker t = new HistoryWorker ();
      t.setListener(this);
      t.start();

    } else if (command == done) {
      startScreen();
    }
  }

  private void startScreen () {
    Form f = new Form ("Get content");
    urlField = new TextField ("URL: ", "", 30, TextField.ANY);
    f.append (urlField);
    f.addCommand (fetch);
    f.addCommand (exit);
    f.setCommandListener(this);
    display.setCurrent(f);
  }

  class FetchWorker extends Thread {

    private CommandListener listener;

    public void setListener (CommandListener cl) {
      listener = cl;
    }

    public void run () {
      HttpConnection conn = null;
      DataInputStream din = null;
      ByteArrayOutputStream bos = null;
      try {
        // Get the url
        String url = urlField.getString ();

        // Fetch the remote content to a byte array "buf"
        conn = (HttpConnection) Connector.open(url);
        conn.setRequestMethod(HttpConnection.GET);
        din = conn.openDataInputStream();
        bos = new ByteArrayOutputStream();
        byte[] buf = new byte[256];
        while (true) {
          int rd = din.read(buf, 0, 256);
          if (rd == -1) break;
          bos.write(buf, 0, rd);
        }
        bos.flush();
        buf = bos.toByteArray();

        // Save the history
        store.addRecord(url.getBytes(), 0, url.getBytes().length);

        // Display a new screen
        Form f = new Form ("content");
        f.append(url + "\n");
        f.append(new String(buf));
        f.addCommand(history);
        f.addCommand(done);
        f.setCommandListener (listener);
        display.setCurrent(f);

      } catch (Exception exp) {
        exp.printStackTrace();
      } finally {
        try {
          if (din != null) din.close();
          if (conn != null) conn.close();
          if (bos != null) bos.close();
        } catch (Exception exp) {}
      }
    }

  }

  class HistoryWorker extends Thread {

    private CommandListener listener;

    public void setListener (CommandListener cl) {
      listener = cl;
    }

    public void run () {
      try {
        Form f = new Form ("History");

        RecordEnumeration enu =
                store.enumerateRecords(null, null, false);
        for (; enu.hasNextElement() ;) {
          byte [] data = enu.nextRecord();
          f.append((new String(data)) + "\n");
        }
        enu.destroy();

        f.addCommand(erase);
        f.addCommand(done);
        f.setCommandListener (listener);
        display.setCurrent(f);

      } catch (Exception exp) {
        exp.printStackTrace();
      }
    }

  }

}