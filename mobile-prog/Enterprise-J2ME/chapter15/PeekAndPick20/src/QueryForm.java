import java.util.Vector;
import javax.microedition.lcdui.*;

/**
 * <code>QueryForm</code> is used by PeekAndPick to ask
 * the user for input. The exact contents of the query
 * form are determined by a <code>Vector</code> of
 * parameters that are passed to the constructor or
 * the <code>setParameters()</code> method.
 *
 * @author Jonathan Knudsen
 */
public class QueryForm
    extends Form {
  private Vector mParameters;
  
  /**
   * Creates a <code>QueryForm</code> with an empty
   * parameter list.
   *
   * @param title the form title
   */
  public QueryForm(String title) { super(title); }
  
  /**
   * Creates a <code>QueryForm</code> with the supplied
   * title and parameter list.
   *
   * @param title the form title
   * @param parameters a list of parameters
   * @see Parameter
   */
  public QueryForm(String title, Vector parameters) {
    super(title);
    setParameters(parameters);
  }
  
  /**
   * Sets the list of parameters to be displayed on this
   * form. Each parameter is represented by a
   * <code>TextField</code>.
   *
   * @param parameters a list of parameters
   * @see Parameter
   */
  public void setParameters(Vector parameters) {
    // Remove everything.
    while(size() > 0)
      delete(0);
    
    // Iterate through the parameters. Create an input field for
    // each field that has a prompt.
    mParameters = parameters;
    
    for (int i = 0; i < mParameters.size(); i++) {
      Parameter p = (Parameter)mParameters.elementAt(i);
      if (p.isPrompt()) {
        append(new TextField(p.getPrompt(), p.getValue(), 128, 0));
      }
    }
  }

  /**
   * Returns a list of parameters representing the values
   * entered in this form.
   *
   * @see Parameter
   */
  public Vector getEntries() {
    // Create a Vector to hold our copy of the parameters.
    Vector entries = new Vector(mParameters.size());
    
    // Iterate through the parameters. For each parameter with
    // a prompt, pull the value from the corresponding TextField.
    int fieldIndex = 0;
    for (int i = 0; i < mParameters.size(); i++) {
      Parameter p = (Parameter)mParameters.elementAt(i);
      if (p.isPrompt()) {
        // Retrieve a reference to the corresponding TextField.
        TextField tf = (TextField)get(fieldIndex++);
        // Now create a new Parameter based on the entered value.
        Parameter entryParameter =
            new Parameter(p.getName(), tf.getString());
        entries.addElement(entryParameter);
      }
      else
        entries.addElement(p);
    }

    return entries;
  }
}
