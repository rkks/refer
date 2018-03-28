/* 
 *
 * The contents of this file are subject to the GNU Public License
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License. 
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific terms governing rights and limitations
 * under the License.
 *
 * Developed by Michael Juntao Yuan 2002.
 *
 * */
package MIDlets;
 
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import GoogleAPI.*;

/** 
  * Example use of the GoogleAPI.GoogleSearch class in MIDP. 
  * Get your own Google query key and substitute the value in
  * string variable "licenseKey".
  */
 
public class GoogleTest extends MIDlet implements CommandListener {
 
  private Display display;
  private Command exitCommand;
  private Command doneCommand;
  private Command searchCommand;
  private Command spellCommand;
  private Command cacheCommand;
  private TextField textField;

  private String licenseKey = "fVq5q3jaHD+gCPlhqy6RE1KqHySRrXuf";
  private String endPointURL = "http://api.google.com/search/beta2";
  
  private GoogleSearch gs;

  public GoogleTest () throws Exception {
    // Initialize display and command objects.
    display = Display.getDisplay(this);
    exitCommand = new Command("EXIT", Command.CANCEL, 1);
    doneCommand = new Command("DONE", Command.CANCEL, 1);
    searchCommand = new Command("Search", Command.SCREEN, 1);
    spellCommand = new Command("Spell", Command.SCREEN, 1);
    cacheCommand = new Command("GetCache", Command.SCREEN, 1);
    gs = new GoogleSearch (licenseKey, endPointURL);
  }

  public void startApp() {
    Form form = new Form("Google Test");
    textField = new TextField("Query:", "",
                              80, TextField.ANY);
    form.append( textField );
 
    form.addCommand(exitCommand);
    form.addCommand(searchCommand);
    form.addCommand(spellCommand);
    form.addCommand(cacheCommand);
 
    // The CommandListener is this MIDlet itself.
    form.setCommandListener( (CommandListener) this);
 
    // Display the form on the LCD screen.
    display.setCurrent(form);
  }

  public void pauseApp() {
  }
 
  public void destroyApp(boolean unconditional) {
  }
 
  public void commandAction(Command command, Displayable screen) {
    if (command == exitCommand) {
      destroyApp(false);
      notifyDestroyed();

    } else if (command == doneCommand) {
      startApp();

    } else if (command == spellCommand) {
      try {
//        String spellSugg = gs.spellCheck2 (textField.getString ());
        String spellSugg = gs.spellCheck (textField.getString ());
        Form form = new Form ("Google Spell Suggestion"); 
        form.append (new StringItem ("Suggest: ", spellSugg));
        form.addCommand (doneCommand);
        form.setCommandListener ((CommandListener) this);
        display.setCurrent(form);

      } catch (Exception e) {
        e.printStackTrace();
      }

    } else if (command == cacheCommand) {
      try {
        byte [] cache = gs.getCache (textField.getString ());

        // max display length so that textBox will not be overflowed.
        int maxlength = 8192;
        TextBox textBox = new TextBox ("Google cache page",
                                       "", maxlength, TextField.ANY);
        maxlength = textBox.getMaxSize ();
        String cacheString = "";
        if (cache.length > maxlength) {
          cacheString = new String (cache, 0, maxlength - 1 );
        } else {
          cacheString = new String (cache);
        }
        textBox.setString (cacheString);
        textBox.addCommand (doneCommand);
        textBox.setCommandListener ((CommandListener) this);
        display.setCurrent(textBox);
 
      } catch (Exception e) {
        e.printStackTrace();
      }

    } else if (command == searchCommand) {
      try {
        int startIndex = 0;
        SearchResults sr = gs.search (textField.getString (), startIndex);

        Form form = new Form ("Google Search Results"); 
        form.append (new StringItem ("Total:", Integer.toString (sr.estimatedTotalResultsCount)));
        form.append (new StringItem ("Time:", sr.searchTime + "s"));
        for ( int i = 0; i < sr.getSize (); i++ ) {
          form.append (new StringItem ("Result: ", Integer.toString (startIndex+i)));
          form.append (new StringItem ("Title:", (String) sr.titles.elementAt(i)));
          form.append (new StringItem ("Size:", (String) sr.sizes.elementAt(i)));
          form.append (new StringItem ("URL:", (String) sr.urls.elementAt(i)));
        }

        form.addCommand (doneCommand);
        form.setCommandListener ((CommandListener) this);
        display.setCurrent(form);

      } catch (Exception e) {
        e.printStackTrace();
      }

    } else {
      // Do nothing.
    }
  }

}
