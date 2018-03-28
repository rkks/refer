package util;

import java.util.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;

/**
 * This class represents key and value pairs that are
 * stored in a record store. You can instantiate a
 * <code>Preferences</code> object by passing a record store name
 * to the constructor. You can add key and value pairs
 * using the <code>put()</code> method and retrieve values by passing
 * a key name to <code>get()</code>.
 *
 * @see util.PreferencesEditor
 * @author Jonathan Knudsen
 */
public class Preferences {
  private String mRecordStoreName;
  
  private Hashtable mHashtable;
  private Hashtable mTypes;
  
  /**
   * Creates a new <code>Preferences</code> objects by reading key and
   * value pairs from the named record store. If the record
   * store is empty or nonexistant, an empty
   * <code>Preferences</code> will be created.
   *
   * @param recordStoreName the name of a record store.
   */
  public Preferences(String recordStoreName) {
    mRecordStoreName = recordStoreName;
    mHashtable = new Hashtable();
    load();
  }
  
  /**
   * Returns the preferences value for the given key.
   */
  public String get(String key) {
    return (String)mHashtable.get(key);
  }
  
  /**
   * Add a new key and value pair to this object.
   */
  public void put(String key, String value) {
    if (value == null) value = "";
    mHashtable.put(key, value);
  }
  
  /**
   * If there is no entry for this key, this method
   * puts a new entry with the specified value.
   */
  public void putIfNull(String key, String value) {
    if (get(key) == null)
      put(key, value);
  }
  
  /**
   * Add the contents of the supplied
   * <code>Hashtable</code> to this
   * <code>Preferences</code> object, possibly overwriting
   * values.
   */
  public void set(Hashtable hashtable) {
    Enumeration keys = hashtable.keys();
    while (keys.hasMoreElements()) {
      String key = (String)keys.nextElement();
      String value = (String)hashtable.get(key);
      mHashtable.put(key, value);
    }
  }
  
  /**
   * Returns the hashtable that is used internally.
   */
  public Hashtable getHashtable() {
    return mHashtable;
  }
  
  private boolean load() {
    boolean success = true;
    
    RecordStore rs = null;
    RecordEnumeration re = null;
    try {
      rs = RecordStore.openRecordStore(mRecordStoreName, false);
      if (rs.getNumRecords() == 0)
        throw new RecordStoreNotFoundException();
      re = rs.enumerateRecords(null, null, false);
      while (re.hasNextElement()) {
        byte[] raw = re.nextRecord();
        String pref = new String(raw);
        // Parse out the name.
        int index = pref.indexOf('|');
        String name = pref.substring(0, index);
        String value = pref.substring(index + 1);
        put(name, value);
      }
    }
    catch (RecordStoreException rse) {
      success = false;
    }
    finally {
      try {
        if (re != null) re.destroy();
        if (rs != null) rs.closeRecordStore();
      }
      catch (RecordStoreException rse) {}
    }
    return success;
  }
  
  /**
   * This method saves the current <code>Preferences</code>
   * values to the same record store name that was passed
   * to the constructor.
   */
  public void save() {
    RecordStore rs = null;
    RecordEnumeration re = null;
    try {
      rs = RecordStore.openRecordStore(mRecordStoreName, true);
      re = rs.enumerateRecords(null, null, false);
      
      // First remove all records, a little clumsy.
      while (re.hasNextElement()) {
        int id = re.nextRecordId();
        rs.deleteRecord(id);
      }
      
      // Now save the preferences records.
      Enumeration keys = mHashtable.keys();
      while (keys.hasMoreElements()) {
        String key = (String)keys.nextElement();
        String value = get(key);
        String pref = key + "|" + value;
        byte[] raw = pref.getBytes();
        rs.addRecord(raw, 0, raw.length);
      }
    }
    catch (RecordStoreException rse) {
    }
    finally {
      try {
        if (re != null) re.destroy();
        if (rs != null) rs.closeRecordStore();
      }
      catch (RecordStoreException rse) {}
    }
  }
}
