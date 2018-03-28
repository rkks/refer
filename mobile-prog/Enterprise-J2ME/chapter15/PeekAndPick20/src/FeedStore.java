import java.util.*;

import javax.microedition.midlet.MIDlet;
import javax.microedition.rms.*;

/**
 * <code>FeedStore</code> represents a list of <code>Feed</code>s that are stored in
 * a recordstore. <code>Feed</code>s can also be loaded from application
 * properties (as defaults).
 *
 * To create a <code>FeedStore</code>, specify the <code>RecordStore</code> name to
 * be used and a base property name to be used for
 * default values.
 *
 * @see Feed
 * @author Jonathan Knudsen
 */
public class FeedStore
    implements RecordComparator {
  private String mRecordStoreName;
  private String mPropertyBaseName;
  private Vector mFeeds;
  private MIDlet mMIDlet;
  
  /**
   * Reads feeds from the named record store. If the
   * recordstore does not exist, feeds are initialized
   * from system properties based on the base property
   * name. The feed property names are propertyBaseName.1,
   * propertyBaseName.2, propertyBaseName.3, and so on.
   * Properties are retrieved from the supplied MIDlet.
   *
   * @param recordStoreName the name of the record store
   *     from which feeds should be loaded
   * @param propertyBaseName a base property name that will
   *     be used to load default feeds if the record store
   *     is not present
   * @param midlet the MIDlet that will be used to retrieve
   *     property values
   */
  public FeedStore(String recordStoreName,
      String propertyBaseName, MIDlet midlet) {
    mRecordStoreName = recordStoreName;
    mPropertyBaseName = propertyBaseName;
    mMIDlet = midlet;
    load();
  }
  
  /**
   * Returns the number of <code>Feed</code>s in this
   * <code>FeedStore</code>.
   */
  public int size() { return mFeeds.size(); }
  
  /**
   * Returns the <code>Feed</code> at the given index.
   *
   * @param i a valid <code>Feed</code> index
   */
  public Feed getFeed(int i) {
    return (Feed)mFeeds.elementAt(i);
  }
  
  /**
   * Adds a new <code>Feed</code>.
   *
   * @param f the <code>Feed</code> to add
   */
  public void add(Feed f) {
    mFeeds.addElement(f);
  }
  
  /**
   * Replaces a <code>Feed</code>.
   *
   * @param oldFeed the <code>Feed</code> to replace
   * @param newFeed the new <code>Feed</code> object
   */
  public void replace(Feed oldFeed, Feed newFeed) {
    int i = mFeeds.indexOf(oldFeed);
    mFeeds.setElementAt(newFeed, i);
  }
  
  /**
   * Removes a <code>Feed</code>.
   *
   * @param f the <code>Feed</code> to remove
   */
  public void remove(Feed f) {
    mFeeds.removeElement(f);
  }

  /**
   * Compares two raw records.
   *
   * @param rec1 the first record
   * @param rec2 the second record
   * @return <code>RecordComparator.PRECEDES</code> if
   *        rec1 should come before rec2<br>
   *    <code>RecordComparator.FOLLOWS</code> if
   *        rec1 should come after rec2<br>
   *    <code>RecordComparator.EQUIVALENT</code> if
   *        rec1 is equivalent to rec2<br>
   */
  public int compare(byte[] rec1, byte[] rec2) {
    int length = Math.min(rec1.length, rec2.length);
    for (int i = 0; i < length; i++) {
      if (rec1[i] < rec2[i]) return PRECEDES;
      else if (rec1[i] > rec2[i]) return FOLLOWS;
    }
    return EQUIVALENT;
  }

  private void load() {
    mFeeds = new Vector();
    try {
      loadFeedsFromRecordStore(mRecordStoreName);
    }
    catch (RecordStoreNotFoundException rsnfe) {
      loadFeedsFromProperties(mPropertyBaseName);
      save();
    }
    catch (RecordStoreException rse) {
    }
  }
  
  private void loadFeedsFromRecordStore(String name)
      throws RecordStoreException {
    mFeeds.removeAllElements();
    RecordStore rs = null;
    RecordEnumeration re = null;
    try {
      rs = RecordStore.openRecordStore(name, false);
      if (rs.getNumRecords() == 0)
        throw new RecordStoreNotFoundException();
      re = rs.enumerateRecords(null, this, false);
      while (re.hasNextElement()) {
        byte[] raw = re.nextRecord();
        Feed f = rawToFeed(raw);
        mFeeds.addElement(f);
      }
    }
    finally {
      if (re != null) re.destroy();
      if (rs != null) rs.closeRecordStore();
    }
  }
  
  /**
   * Clears this <code>FeedStore</code> and
   * loads feeds from using the specified property base
   * name. Property names are created by appending
   * <i>.n</i>, where <i>n</i> is an integer starting at
   * 1 and counting up until the property is not present.
   * For example, passing a <code>baseName</code> of
   * &quot;boo&quot; would result in an attempt to load
   * <code>Feed</code>s from properties named
   * &quot;boo.1&quot;, &quot;boo.2&quot;, and so forth.
   *
   * @param baseName the base property name
   */
  public void loadFeedsFromProperties(String baseName) {    
    mFeeds.removeAllElements();
    int index = 1;
    boolean trucking = true;
    while (trucking) {
      String propertyName = baseName + "." + index++;
      String feedString = mMIDlet.getAppProperty(propertyName);
      if (feedString == null)
        trucking = false;
      else {
        Feed f = Feed.create(feedString);
        mFeeds.addElement(f);
      }
    }
  }
  
  /**
   * Saves feeds to the record store.
   */
  public void save() {
    String name = mRecordStoreName;
    RecordStore rs = null;
    RecordEnumeration re = null;
    try {
      rs = RecordStore.openRecordStore(name, true);
      re = rs.enumerateRecords(null, null, false);
      boolean[] found = new boolean[mFeeds.size()];
      while (re.hasNextElement()) {
        int id = re.nextRecordId();
        // Stupid code to get the associated record.
        byte[] raw = null;
        if (re.hasPreviousElement()) {
          re.previousRecordId();
          raw = re.nextRecord();
        }
        else if (re.hasNextElement()) {
          re.nextRecordId();
          raw = re.previousRecord();
        }
        else {
          re.reset();
          raw = re.nextRecord();
        }
        // Look for a match in our internal list.
        boolean recordFound = false;
        for (int i = 0; i < mFeeds.size(); i++) {
          byte[] existingRaw = feedToRaw(i);
          if (byteEquals(raw, existingRaw)) {
            found[i] = true;
            recordFound = true;
            break;
          }
        }
        // Remove records that have no match.
        if (recordFound == false) {
          rs.deleteRecord(id);
        }
      }
      
      // Now look through mFeeds. Anything that wasn't in
      // the recordstore should be added.
      for (int i = mFeeds.size() - 1; i >= 0; i--) {
        if (found[i] == false) {
          byte[] raw = feedToRaw(i);
          rs.addRecord(raw, 0, raw.length);
        }
      }
    }
    catch (RecordStoreException rse) {
      System.out.println(rse);
    }
    finally {
      try {
        if (re != null) re.destroy();
        if (rs != null) rs.closeRecordStore();
      }
      catch (RecordStoreException rse) {}
    }
  }
  
  private byte[] feedToRaw(int i) {
    Feed f = (Feed)mFeeds.elementAt(i);
    byte[] raw = f.getRaw().getBytes();
    byte[] ordered = new byte[raw.length + 1];
    ordered[0] = (byte)i; // Store the position here.
    System.arraycopy(raw, 0, ordered, 1, raw.length);
    return ordered;
  }
  
  private Feed rawToFeed(byte[] raw) {
    String s = new String(raw, 1, raw.length - 1);
    return Feed.create(s);
  }
  
  private boolean byteEquals(byte[] one, byte[] two) {
    if (one.length != two.length) return false;
    for (int i = 0; i < one.length; i++)
      if (one[i] != two[i]) return false;
    return true;
  }
}
