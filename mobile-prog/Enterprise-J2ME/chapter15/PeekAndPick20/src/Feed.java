import java.util.*;

/**
 * Feed represents an RSS or RDF feed. Feed supports a form
 * of serialization where instances can be converted into
 * a string using getRaw() and recreated using the static
 * create() method.
 *
 * Feeds consist of a displayable name, a base URL, and 0 or
 * more parameters.
 *
 * The URL corresponding to a Feed is retrieved using
 * getURL(). If a Feed includes parameters that are defined
 * by user input, then the alternate form of getURL() should
 * be used. This getURL() method accepts a Vector of
 * Parameter objects which have both a name and value
 * defined.
 *
 * @author Jonathan Knudsen
 * @see Parameter
 */
public class Feed {
  private static final char kDelimiter = '|';
  
  /**
   * This factory method creates a <code>Feed</code> object from the
   * given <code>String</code>. It parses strings with the following
   * form:
   *
   * name|baseURL[|parameter1|parameter2|...]
   *
   * See the <code>Parameter</code> documentation for the serialized
   * form of parameters.
   *
   * @param s a string representing a feed
   */
  public static Feed create(String s) {
    String name, baseUrl;
    Vector parameters = new Vector();
    
    int index = 0, delimiter;
    
    // Parse name.
    delimiter = s.indexOf(kDelimiter, index);
    if (delimiter == -1)
      throw new IllegalArgumentException("Could not parse name.");
    name = s.substring(index, delimiter);
    index = delimiter + 1;
    
    boolean trucking = true;
    
    // Parse URL.
    delimiter = s.indexOf(kDelimiter, index);
    if (delimiter == -1) {
      baseUrl = s.substring(index);
      trucking = false;
    }
    else {
      baseUrl = s.substring(index, delimiter);
      index = delimiter + 1;
    }
    
    // Parse parameters.
    while (trucking) {
      delimiter = s.indexOf(kDelimiter, index);
      if (index >= s.length()) trucking = false;
      else {
        if (delimiter == -1) delimiter = s.length();
        String parameterString = s.substring(index, delimiter);
        Parameter parameter = Parameter.create(parameterString);
        parameters.addElement(parameter);
        index = delimiter + 1;
      }
    }
    
    return new Feed(name, baseUrl, parameters);
  }
  
  private String mName, mBaseURL;
  private Vector mParameters;
  
  /**
   * Creates a new <code>Feed</code> with the specified
   * name and base URL, and an empty list of parameters.
   *
   * @param name a human-readable name for the feed
   * @param baseUrl a URL for the feed without any
   *    parameters
   */
  public Feed(String name, String baseUrl) {
    this (name, baseUrl, new Vector());
  }
  
  /**
   * Creates a new <code>Feed</code> with the specified
   * name, base URL, and list of parameters.
   *
   * @param name a human-readable name for the feed
   * @param baseUrl a URL for the feed without any
   *     parameters
   * @param parameters a list of <code>Parameter</code>s
   *     associated with this feed
   */
  protected Feed(String name, String baseUrl, Vector parameters) {
    mName = name;
    mBaseURL = baseUrl;
    mParameters = parameters;
  }
  
  public String getName() { return mName; }
  public Vector getParameters() { return mParameters; }

  /**
   * This method returns the URL represented by the <code>Feed</code>.
   * Use this method if the <code>Feed</code> contains no <code>Parameter</code>s that
   * require user prompting.
   */
  public String getURL() {
    return getURL(null);
  }
  
  /**
   * Use this form of <code>getURL()</code> if the user is prompted for
   * some or all <code>Parameter</code>s. The <code>Vector</code> supplied should
   * contain <code>Parameter</code> objects that have both a name and
   * a value defined.
   */
  public String getURL(Vector entries) {
    if (entries == null) entries = mParameters;

    int count = entries.size();
    if (count == 0) return mBaseURL;
    
    StringBuffer buffer = new StringBuffer(mBaseURL);
    buffer.append('?');
    for (int i = 0; i < count; i++) {
      if (i != 0)
        buffer.append('&');
      Parameter parameter = (Parameter)entries.elementAt(i);
      buffer.append(parameter.getName());
      buffer.append('=');
      buffer.append(parameter.getValue());
    }
    return buffer.toString();
  }
  
  /**
   * This method returns the serialized <code>Feed</code> object. The
   * output of this method can be fed back into <code>create()</code> to
   * recreate <code>Feed</code> objects.
   */
  public String getRaw() {
    StringBuffer buffer = new StringBuffer();
    
    buffer.append(getName());
    buffer.append(kDelimiter);
    buffer.append(mBaseURL);
    for (int i = 0; i < mParameters.size(); i++) {
      buffer.append(kDelimiter);
      Parameter p = (Parameter)mParameters.elementAt(i);
      buffer.append(p.getRaw());
    }
    
    return buffer.toString();
  }
  
  /**
   * This method returns true if any of the <code>Feed</code>'s
   * <code>Parameter</code>s require user prompting.
   */
  public boolean hasPrompts() {
    for (int i = 0; i < mParameters.size(); i++) {
      Parameter p = (Parameter)mParameters.elementAt(i);
      if (p.isPrompt()) return true;
    }
    return false;
  }
}


