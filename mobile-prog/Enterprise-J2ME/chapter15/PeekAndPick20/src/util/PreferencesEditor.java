package util;

import java.util.*;

import javax.microedition.lcdui.*;

/**
 * This <code>Form</code> subclass provides a user
 * interface for editing <code>Preferences</code> objects.
 * This class supports both text and choice data. Basic
 * usage is as follows:
 * <ul>
 * <li>Create a new <code>PreferencesEditor</code>, passing
 *   in a list of key values representing items that will
 *   not be shown.</li>
 * <li>Call <code>setType()</code> to set the type of
 *   each item and its constraints.</li>
 * <li>Pass a <code>Preferences</code> object to
 *   <code>initialize()</code> to set up the form.</li>
 * <li>Show the <code>PreferencesEditor</code> and allow
 *   the user to make changes.</li>
 * <li>Retrieve the edited values using
 *   <code>extract()</code>. Update the original
 *   <code>Preferences</code> object using the edited
 *   values.</li>
 * </ul>
 *
 * @see util.Preferences
 * @author Jonathan Knudsen
 */
public class PreferencesEditor
    extends Form {
  public static final int kText = 1;
  public static final int kChoice = 2;
  
  private String[] mNoShows;
  private Hashtable mTypes;
  private Hashtable mConstraints;
  
  /**
   * Creates a new editor.
   *
   * @param noShows a string array containing the names
   *     of fields that should not be editable and will
   *     not be shown on this editor
   */
  public PreferencesEditor(String[] noShows) {
    super("Preferences");
    mNoShows = noShows;
    mTypes = new Hashtable();
    mConstraints = new Hashtable();
  }

  /**
   * This method extracts values from the form fields and
   * returns the results as a <code>Hashtable</code>.
   *
   * @return a hashtable containing key and value pairs
   *     representing the data entered in this form
   */
  public Hashtable extract() {
    Hashtable hashtable = new Hashtable();
    // Move fields from the form into the Hashtable.
    for (int i = 0; i < size(); i++) {
      Item item = (Item)get(i);
      if (item instanceof ChoiceGroup) {
        ChoiceGroup cg = (ChoiceGroup)item;
        String key = cg.getLabel();
        String value = 
            cg.getString(cg.getSelectedIndex());
        hashtable.put(key, value);
      }
      else if (item instanceof TextField) {
        TextField textField = (TextField)item;
        String key = textField.getLabel();
        String value = textField.getString(); 
        hashtable.put(key, value);
      }
    }

    return hashtable;
  }
  
  /**
   * Creates this form based on the keys and values in
   * the given <code>Preferences</code> object. Key values
   * are used as
   * labels on user interface items. If types and
   * constraints have been set up for certain keys they
   * are used to determine the type and content of the
   * user interface items.
   */
  public void initialize(Preferences p) {
    // Remove everything.
    while(size() > 0)
      delete(0);

    // Add fields from the supplied Preferences.
    Hashtable hashtable = p.getHashtable();
    Enumeration keys = hashtable.keys();
    while (keys.hasMoreElements()) {
      String key = (String)keys.nextElement();
      String value = (String)hashtable.get(key);
      Object typeObject = mTypes.get(key);
      int type = kText;
      if (typeObject != null)
        type = ((Integer)typeObject).intValue();
      Object constraints = mConstraints.get(key);
      if (isVisible(key)) {
        switch (type) {
          case kChoice:
            ChoiceGroup cg = new ChoiceGroup(key,
                Choice.EXCLUSIVE,
                (String[])constraints,
                null);
            append(cg);
            // Select the appropriate item.
            for (int i = 0; i < cg.size(); i++) {
              boolean selected = cg.getString(i).equals(value);
              if (selected == true) 
                cg.setSelectedIndex(i, selected);
            }                
            break;
          case kText:
          default:
            int c = 0;
            if (constraints != null)
              c = ((Integer)constraints).intValue();
            TextField tf = new TextField(key, value, 128, c);
            append(tf);
            break;
        }
      }
    }
  }
  
  private boolean isVisible(String key) {
    if (mNoShows == null) return true;
    
    for (int i = 0; i < mNoShows.length; i++)
      if (mNoShows[i].equals(key))
        return false;
    return true;
  }

  /**
   * Sets the type and constaints for a given key.
   *
   * @param key a key
   * @param type should be one of <code>kChoice</code>
   *     or <code>kText</code>
   * @param constraints can be a string array for
   *     <code>kChoice</code> type keys
   */
  public void setType(String key, int type,
      Object constraints) {
    mTypes.put(key, new Integer(type));
    if (constraints != null)
      mConstraints.put(key, constraints);
  }
  
  /**
   * Sets the type and constaints for a given key.
   *
   * @param key a key
   * @param type should be one of <code>kChoice</code>
   *     or <code>kText</code>
   * @param constraints can be an integer representing
   *     <code>TextField</code> constraints
   */
  public void setType(String key, int type,
      int constraints) {
    setType(key, type, new Integer(constraints));
  }
}
