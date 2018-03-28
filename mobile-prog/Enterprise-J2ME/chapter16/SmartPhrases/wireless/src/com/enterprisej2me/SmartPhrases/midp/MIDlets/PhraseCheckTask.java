package com.enterprisej2me.SmartPhrases.midp.MIDlets;
 
import java.util.*;
import java.io.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import com.enterprisej2me.SmartPhrases.midp.SearchUtils.*;

public class PhraseCheckTask extends BackgroundTask {

  static public String cacheName = "PhraseStore";

  public PhraseCheckTask ( Search s ) { 
    super ();
    this.searchMIDlet = s;
    this.title = "Checking usage stat ...";
  }

  public void runTask () throws Exception {
    String searchStr = searchMIDlet.searchStr;
    PhraseCache pc = getCache(searchStr);
    
    if ( pc == null ) {
      String googleStr = searchMIDlet.googleStr;
      pc = new PhraseCache ();
      GoogleResults sr = searchMIDlet.gs.search (googleStr, 0);

      pc.freq = Integer.toString (sr.estimatedTotalResultsCount);
      for ( int i = 0; i < sr.getSize (); i++ ) {
        String url = (String) sr.urls.elementAt(i);
        // Do not count leading http://
        String hostName = url.substring(7, url.indexOf('/',7));
        pc.hostNames.addElement( hostName );
      }
      
      saveCache( searchStr, pc );
    }

    Form form = new Form ("Usage stat:");
    form.append(searchStr + "\n\n");
    form.append("It has appeared on the Internet for " 
                + pc.freq + " times. Please click MORE "
                + " for Google suggested alternative spelling\n\n");
    form.append("Sample web sites are: \n");
    for (Enumeration e = pc.hostNames.elements(); e.hasMoreElements();) {
      form.append ((String) e.nextElement());
    } 

    form.setCommandListener ((CommandListener) searchMIDlet);
    form.addCommand (searchMIDlet.backCommand );
    form.addCommand (searchMIDlet.phraseSuggCommand );

    nextScreen = form;
    return;
  }

  private PhraseCache getCache (String query) throws Exception { 
    PhraseCache pc = null;
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
        pc = new PhraseCache();
        pc.freq = din.readUTF();
        int size = din.readInt();
        for (int i = 0; i < size; i++) {
          pc.hostNames.addElement( (String) din.readUTF() );
        }
        break;
      }
      din.close();
      bin.close();
    }
    enu.destroy();
    cacheStore.closeRecordStore();
    return pc;
  }

  private void saveCache (String query, PhraseCache pc) throws Exception {
    RecordStore cacheStore = RecordStore.openRecordStore(cacheName, true);
    ByteArrayOutputStream bout = new ByteArrayOutputStream ();
    DataOutputStream dout = new DataOutputStream (bout);
    dout.writeUTF( query );
    dout.writeUTF( pc.freq );
    dout.writeInt( pc.hostNames.size() );
    for (Enumeration e = pc.hostNames.elements(); e.hasMoreElements();) {
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

class PhraseCache {

  public PhraseCache () {
    freq = "";
    hostNames = new Vector ();
  }

  public String freq;
  public Vector hostNames;
}
