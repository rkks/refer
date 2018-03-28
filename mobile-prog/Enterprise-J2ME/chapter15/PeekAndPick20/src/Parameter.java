import java.util.*;

/**
 * Parameter represents a Feed parameter. It consists of a
 * name, an optional value, and an optional prompt string.
 * The name and value will be embedded directly in URLs
 * generated from a Feed, while the prompt string will be
 * shown to the user.
 *
 * Parameters without a prompt string are used unchanged.
 * If a Parameter has a prompt string, the value (if
 * present) is shown as a default.
 *
 * @author Jonathan Knudsen
 */
public class Parameter {
  protected static final char kDelimiter = ';';
  
  /**
   * This factory method creates a Parameter object from the
   * given String. It parses strings of the following form:
   *
   * name;value[;prompt]
   *
   * @param s the serialized <code>Parameter</code>
   */
  public static Parameter create(String s) {
    String name, value, prompt = null;
    
    int index = 0, delimiter;
    
    // Parse name.
    delimiter = s.indexOf(kDelimiter, index);
    if (delimiter == -1)
      throw new IllegalArgumentException("Could not parse name.");
    name = s.substring(index, delimiter);
    index = delimiter + 1;
    
    // Parse value.
    delimiter = s.indexOf(kDelimiter, index);
    if (delimiter == -1) delimiter = s.length();
    value = s.substring(index, delimiter);
    index = delimiter + 1;
    
    // Parse prompt.
    if (index < s.length())
      prompt = s.substring(index);
    
    return new Parameter(name, value, prompt);
  }
  
  private String mName, mValue, mPrompt;
  
  /**
   * Creates a <code>Parameter</code> with no prompt string.
   *
   * @param name a human-readable name
   * @param value a default value
   */ 
  protected Parameter(String name, String value) {
    this(name, value, null);
  }
  
  /**
   * Creates a <code>Parameter</code>.
   *
   * @param name a human-readable name
   * @param value a default value
   * @param prompt the human-readable prompt string
   */ 
  protected Parameter(String name, String value, String prompt) {
    mName = name;
    mValue = value;
    mPrompt = prompt;
  }
  
  public String getName() { return mName; }
  public String getValue() { return mValue; }
  public String getPrompt() { return mPrompt; }
  
  /**
   * Returns <code>true</code> if this
   * <code>Parameter</code> has a prompt string.
   */
  public boolean isPrompt() {
    return (mPrompt != null) && (mPrompt.length() > 0);
  }
  
  /**
   * This method returns a serialized representation of
   * the Parameter. The output of this method can be fed
   * back into create() to recreate Parameter objects.
   */
  public String getRaw() {
    StringBuffer buffer = new StringBuffer(getName());
    buffer.append(kDelimiter);
    buffer.append(getValue());
    if (mPrompt != null) {
      buffer.append(kDelimiter);
      buffer.append(mPrompt);
    }
    return buffer.toString();
  }
}

