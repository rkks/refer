package com.enterprisej2me.SmartPhrases.midp.MIDlets;
 
import java.util.*;
import java.io.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import com.enterprisej2me.SmartPhrases.midp.SearchUtils.*;

public class WordCheckTask extends BackgroundTask {

  static public String cacheName = "WordDefStore";

  public WordCheckTask ( Search s ) { 
    super ();
    this.searchMIDlet = s;
    this.title = "Checking Definition ...";
  }

  public void runTask () throws Exception {
    String searchStr = searchMIDlet.searchStr;
    String def = getCache(searchStr);
    if ( def == null ) {
      def = searchMIDlet.ds.getWordMeaning (searchStr);
      saveCache( searchStr, def );
    }
    if ( def.trim().equals("") ) {
      def = "No definition found. Please check MORE spelling";
    }

    Form form = new Form ("Definitions:");
    form.append(searchStr + "\n\n");
    form.append(def);
    form.addCommand (searchMIDlet.backCommand);
    form.addCommand (searchMIDlet.wordSuggCommand);
    form.setCommandListener ((CommandListener) searchMIDlet);
    nextScreen = form;
    return;
  }

  private String getCache( String query ) throws Exception {
    String def = null;
    RecordStore cacheStore = RecordStore.openRecordStore(cacheName, true);
    ByteArrayInputStream bin;
    DataInputStream din;
    RecordEnumeration enu;
    try {
      // If the store is empty, this method will fail.
      // Is this a bug?
      enu = cacheStore.enumerateRecords(null, null, false);
    } catch (Exception e) {
      return null;
    }
    for (; enu.hasNextElement() ;) {
      bin = new ByteArrayInputStream( enu.nextRecord() );
      din = new DataInputStream( bin );
      String s = din.readUTF();
      if ( query.equals(s) ) {
        def = din.readUTF();
        break;
      }
      din.close();
      bin.close();
    }
    enu.destroy();
    cacheStore.closeRecordStore();
    return def;
  }

  private void saveCache( String query, String def ) throws Exception {
    RecordStore cacheStore = RecordStore.openRecordStore(cacheName, true);
    ByteArrayOutputStream bout = new ByteArrayOutputStream ();
    DataOutputStream dout = new DataOutputStream (bout);
    dout.writeUTF( query );
    dout.writeUTF( def );
    dout.writeUTF( "" );
    dout.flush();
    byte [] toSave = bout.toByteArray();
    int id = cacheStore.addRecord(toSave, 0, toSave.length);
    dout.close();
    bout.close();
    cacheStore.closeRecordStore();
    return;
  }

}
