package com.enterprisej2me.SmartPhrases.midp.MIDlets;
 
import java.util.*;
import java.io.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import com.enterprisej2me.SmartPhrases.midp.SearchUtils.*;

public class WordSuggTask extends BackgroundTask {

  static public String cacheName = "WordSuggStore";

  public WordSuggTask ( Search s ) { 
    super ();
    this.searchMIDlet = s;
    this.title = "Checking spell suggestions ...";
  }

  public void runTask () throws Exception {
    String searchStr = searchMIDlet.searchStr;
    Vector wordSuggVec = getCache(searchStr);
    if ( wordSuggVec == null ) {
      wordSuggVec = searchMIDlet.ds.getSpellSugg(searchStr);
      saveCache( searchStr, wordSuggVec );
    }

    int size = wordSuggVec.size ();
    if ( size == 0 ) {
      Form form = new Form ("No Suggestion");
      form.append ("There is no suggestion for "
                         + searchMIDlet.searchStr);
      form.addCommand (searchMIDlet.backCommand);
      form.setCommandListener ((CommandListener) searchMIDlet);
      nextScreen = form;
    } else {
      String [] listItems = new String [size];
      for ( int i = 0; i < size; i++ ) {
        listItems[i] = (String) wordSuggVec.elementAt(i);
      }
      nextScreen = new List("Suggestions", List.IMPLICIT, listItems, null);
      nextScreen.addCommand (searchMIDlet.selectSuggCommand);
      nextScreen.addCommand (searchMIDlet.backCommand);
      nextScreen.setCommandListener ((CommandListener) searchMIDlet);
      searchMIDlet.wordSuggList = (List) nextScreen;
    }

    return;
  }

  private Vector getCache (String query) throws Exception {
    Vector sugg = null;
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
        int size = din.readInt();
        sugg = new Vector ();
        for (int i = 0; i < size; i++) {
          sugg.addElement( (String) din.readUTF() );
        }
        break;
      }
      din.close();
      bin.close();
    }
    enu.destroy();
    cacheStore.closeRecordStore();
    return sugg;
  }

  private void saveCache (String query, Vector sugg) throws Exception {
    RecordStore cacheStore = RecordStore.openRecordStore(cacheName, true);
    ByteArrayOutputStream bout = new ByteArrayOutputStream ();
    DataOutputStream dout = new DataOutputStream (bout);
    dout.writeUTF( query );
    dout.writeInt( sugg.size() );
    for (Enumeration e = sugg.elements(); e.hasMoreElements();) {
      dout.writeUTF( (String) e.nextElement() );
    }
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
