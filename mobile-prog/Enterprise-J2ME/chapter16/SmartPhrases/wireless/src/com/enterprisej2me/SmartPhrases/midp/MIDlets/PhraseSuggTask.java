package com.enterprisej2me.SmartPhrases.midp.MIDlets;
 
import java.util.*;
import java.io.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import com.enterprisej2me.SmartPhrases.midp.SearchUtils.*;

public class PhraseSuggTask extends BackgroundTask {

  public PhraseSuggTask ( Search s ) { 
    super ();
    this.searchMIDlet = s;
    this.title = "Checking spell suggestions ...";
  }

  public void runTask () throws Exception {
      String googleStr = searchMIDlet.googleStr;
      String spellSugg = searchMIDlet.gs.spellCheck (googleStr);

      if ( spellSugg != null ) {
        // If Google does return a spell suggestion, it is enclosed
        // in &quot; signs because the original query is enclosed
        // in quotes.
 
        // get rid of leading and ending &quot;
        spellSugg = spellSugg.substring(6, spellSugg.length()-6);
        // get rid of + signs
        spellSugg = spellSugg.replace('+', ' ');
        spellSugg = spellSugg.trim();
      }

    Form form = new Form ("Suggestion:");
    form.append(searchMIDlet.searchStr + "\n\n");

    if ( spellSugg == null || "".equals(spellSugg) ) {
      form.append("No suggestion");
      form.addCommand (searchMIDlet.backCommand );
    } else {
      form.append("Spell Suggestion: \n\n" 
                  + spellSugg + "\n\n"
                  + "Click GO to check it");
      form.addCommand (searchMIDlet.backCommand );
      form.addCommand (searchMIDlet.phraseSuggCKCommand );
      searchMIDlet.phraseSuggStr = spellSugg;
    }
    form.setCommandListener ((CommandListener) searchMIDlet);

    nextScreen = form;
    return;
  }

}
