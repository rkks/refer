package com.enterprisej2me.SmartPhrases.midp.MIDlets;
 
import java.util.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;

public class ClearTask extends BackgroundTask {

  public ClearTask ( Search s ) { 
    super ();
    this.searchMIDlet = s;
    this.title = "Clearing cache ...";
  }

  public void runTask () throws Exception {
    try {
      RecordStore.deleteRecordStore( PhraseCheckTask.cacheName );
    } catch (Exception e) {
      e.printStackTrace();
    }
    try {
      RecordStore.deleteRecordStore( WordCheckTask.cacheName );
    } catch (Exception e) {
      e.printStackTrace();
    }
    try {
      RecordStore.deleteRecordStore( WordSuggTask.cacheName );
    } catch (Exception e) {
      e.printStackTrace();
    }

    Form form = new Form ("Results");
    form.append ("Cache cleared");
    form.addCommand (searchMIDlet.backCommand);
    form.setCommandListener ((CommandListener) searchMIDlet);
    nextScreen = form;
    return;
  }
}
