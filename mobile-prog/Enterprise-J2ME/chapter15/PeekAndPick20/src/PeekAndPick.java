import java.io.*;
import java.util.*;
import javax.microedition.io.*;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;
import javax.microedition.rms.*;

import display.*;
import rss.*;
import util.*;

/**
 * <code>PeekAndPick</code> is a <code>MIDlet</code> that
 * controls the PeekAndPick application. It handles all
 * <code>Command</code>s and responds to events from
 * the <code>Worker</code> thread and the RSS/RDF parser.
 *
 * @author Jonathan Knudsen
 */
public class PeekAndPick
    extends MIDlet
    implements CommandListener, WorkerListener,
        ParserListener {
  private static final String kBanner = "PeekAndPick 2.0";
  
  private boolean mInitialized;
  private FeedStore mFeedStore;
  private Preferences mPreferences;
  private Parser mParser;
  private Worker mWorker;
  
  private Display mDisplay;
  private Displayable mLastScreen;
  private Command mExitCommand, mBackCommand,
      mSubmitCommand, mSendCommand, mPrefCommand,
      mSaveCommand, mDetailsCommand, mCancelCommand;
  private Command mAddStoriesCommand;
  
  private List mFeedList;
  private PreferencesEditor mPreferencesEditor;
  private ListCanvas mListCanvas;
  private QueryForm mQueryForm;
  
  private boolean mAddStoriesFlag;
  
  public PeekAndPick() {
    mInitialized = false;
  }
  
  public void startApp() {
    if (mDisplay == null) mDisplay = Display.getDisplay(this);
    if (mInitialized == false) {
      initialize();
      try {
        // mLastScreen is already set by initialize(). Here
        //   we attempt to create the splash screen.
        Displayable splash = new SplashScreen("/splash.png",
            mDisplay, mLastScreen);
        mLastScreen = splash;
      }
      catch (IOException ioe) {}
    }
    mDisplay.setCurrent(mLastScreen);
  }
  
  public void pauseApp() {
    // Save the last visible screen. This will be
    // restored in startApp().
    mLastScreen = mDisplay.getCurrent();
  }
  
  public void destroyApp(boolean unconditional) {}
  
  // CommandListener method
  
  public void commandAction(Command c, Displayable s) {
    if (c == mExitCommand)
      notifyDestroyed();
    else if (c == mBackCommand)
      mDisplay.setCurrent(mFeedList);
    else if (c == mAddStoriesCommand) {
      mAddStoriesFlag = true;
      mDisplay.setCurrent(mFeedList);
    }
    else if (c == List.SELECT_COMMAND) {
      mLastScreen = mDisplay.getCurrent();
      Feed feed = getSelectedFeed();
      if (feed.hasPrompts()) {
        // Prompt for the missing parameters.
        Vector parameters = feed.getParameters();
        createQueryForm(parameters);
        mDisplay.setCurrent(mQueryForm);
      }
      else {
        mListCanvas.setFontSize(mPreferences.get("Font size"));
        if (mAddStoriesFlag == true) mAddStoriesFlag = false;
        else mListCanvas.clear();
        mDisplay.setCurrent(mListCanvas);
        ParseTask task = new ParseTask(mParser, feed, null);
        mWorker.queue(task);
      }
    }
    else if (c == mDetailsCommand && s == mListCanvas) {
      mListCanvas.details();
    }
    else if (c == mSubmitCommand) {
      Feed feed = getSelectedFeed();
      mListCanvas.setFontSize(mPreferences.get("Font size"));
      if (mAddStoriesFlag == true) mAddStoriesFlag = false;
      else mListCanvas.clear();
      mDisplay.setCurrent(mListCanvas);
      // Use the form entries.
      ParseTask task = new ParseTask(mParser,
          feed, mQueryForm.getEntries());
      mWorker.queue(task);
    }
    else if (c == mSendCommand) {
      mLastScreen = mDisplay.getCurrent();
      sendMail();
    }
    else if (c == mPrefCommand) {
      getPreferencesEditor();
      mPreferencesEditor.initialize(mPreferences);
      
      // Show the preferences screen.
      mDisplay.setCurrent(mPreferencesEditor);
    }
    else if (c == mSaveCommand) {
      // Validate the email address.
      Hashtable hashtable = mPreferencesEditor.extract();
      if (validate(hashtable) == false) {
        Alert a = new Alert("Sorry",
            "I didn't recognize that email address.",
            null, null);
        a.setTimeout(Alert.FOREVER);
        mDisplay.setCurrent(a, mPreferencesEditor);
        return;
      }

      // Save preferences.
      mDisplay.setCurrent(mFeedList);
      mPreferences.set(hashtable);
      mPreferences.save();
    }
    else if (c == mCancelCommand) {
      // Cancel the current task.
      mWorker.cancel();
    }
  }
  
  // WorkerListener methods
  
  public void exception(WorkerTask r, Exception e) {
    String message = "";
    
    
    if (r instanceof ParseTask)
      message = "Something went wrong while reading " +
          "that feed. Please try again or try a " + 
          "different feed.";
    else if (r instanceof MailTask)
      message = "Something went wrong while trying " +
          "to send email. Please try again or check " + 
          "your email address in the preferences screen.";
    Alert a = new Alert("Error", message,
        null, null);
    a.setTimeout(Alert.FOREVER);
    Displayable next = mFeedList;
    if (mListCanvas.size() > 0) next = mListCanvas;
    mDisplay.setCurrent(a, next);
  }
  
  public void finished(WorkerTask r) {
    if (r instanceof MailTask) {
      MailTask mailTask = (MailTask)r;
      int code = mailTask.getResponseCode();
      if (code == -1) return;
      else if (code < 200) {
        // Succeed silently.
      }
      else {
        Alert a = new Alert("Oops",
            "A problem occurred when sending " +
            "your email: " + mailTask.getResponseMessage(),
            null, null);
        a.setTimeout(Alert.FOREVER);
        mDisplay.setCurrent(a, mListCanvas);
      }
      if (code == 199) {
        String newURL = mailTask.getResponseMessage();
        mPreferences.put("PeekAndPick Server", newURL);
        mPreferences.save();
      }
    }
  }
    
  // ParserListener methods
  
  public void firstItem() {
  }

  public void itemParsed(String title, String link,
      String description) {
    mListCanvas.addItem(title, description, link);
  }
  
  public void finished() {
    // Put up a screen if nothing came back.
    if (mListCanvas.size() == 0) {
      Alert a = new Alert("No results",
          "No items were returned.",
          null, null);
      a.setTimeout(Alert.FOREVER);
      mDisplay.setCurrent(a, mListCanvas);
    }
  }
  
  // Private internal methods
  
  private void initialize() {
    // Pull the feeds out of persistent storage.
    mFeedStore = new FeedStore(
        "PeekAndPick.feeds", "PeekAndPick.feed", this);
    
    // Create commands.
    mExitCommand = new Command("Exit", Command.EXIT, 2);
    mBackCommand = new Command("Back", Command.BACK, 1);
    mSendCommand = new Command("Email", Command.SCREEN, 0);
    mPrefCommand = new Command("Prefs",
        Command.SCREEN, 2);
    mSaveCommand = new Command("Save", Command.SCREEN, 1);
    mDetailsCommand =
        new Command("Details", Command.SCREEN, 0);
    mCancelCommand =
        new Command("Cancel", Command.CANCEL, 0);
    mAddStoriesCommand =
        new Command("Add stories", Command.SCREEN, 0);
    
    // Create the main screen, a list of feeds.
    mFeedList = new List(kBanner, List.IMPLICIT);
    for (int i = 0; i < mFeedStore.size(); i++) {
      Feed feed = mFeedStore.getFeed(i);
      mFeedList.append(feed.getName(), null);
    }
    mFeedList.addCommand(mExitCommand);
    mFeedList.addCommand(mPrefCommand);
    mFeedList.setCommandListener(this);
    
    // Load preferences.
    mPreferences = new Preferences("PeekAndPick.preferences");
    String email =
        getAppProperty("PeekAndPick.preferences.email");
    String server =
        getAppProperty("PeekAndPick.server.URL");
    mPreferences.putIfNull("Email address", email);
    mPreferences.putIfNull("PeekAndPick Server", server);
    mPreferences.putIfNull("Font size", "Small");
    
    // Create the headline list screen.
    mListCanvas = new ListCanvas(mDisplay, mCancelCommand);
    mListCanvas.addCommand(mBackCommand);
    mListCanvas.addCommand(mSendCommand);
    mListCanvas.addCommand(mDetailsCommand);
    mListCanvas.addCommand(mAddStoriesCommand);
    mListCanvas.setCommandListener(this);
    
    // Create the parser.
    mParser = new kXML12Parser();
    mParser.addParserListener(this);
    
    // Create the worker.
    mWorker = new Worker(mListCanvas);
    mWorker.setWorkerListener(this);

    // Last screen is the feed list by default. But if
    // the preferences aren't valid (e.g. if they're
    // uninitialized), show the preferences screen first.
    mLastScreen = mFeedList;
    if (validate() == false)
      mLastScreen = getPreferencesEditor();
    
    mInitialized = true;
  }

  private Feed getSelectedFeed() {
    // First find the matching feed.
    String name = mFeedList.getString(
        mFeedList.getSelectedIndex());
    Feed feed = null;
    for (int i = 0; i < mFeedStore.size() && feed == null; i++) {
      feed = mFeedStore.getFeed(i);
      if (name.equals(feed.getName()) == false)
        feed = null;
    }
    return feed;
  }

  private void sendMail() {
    if (mListCanvas.hasMarks() == false) {
      Alert a = new Alert("Nothing to send",
          "You haven't marked any items to e-mail. " + 
          "To mark an item, " +
          "press the select button. A small check mark " +
          "will appear next to the current item.",
          null, null);
      a.setTimeout(Alert.FOREVER);
      mDisplay.setCurrent(a, mListCanvas);
      return;
    }

    String email = mPreferences.get("Email address");
    String url = mPreferences.get("PeekAndPick Server");
    
    // Send some information for the server log.
    String log = "";

    // Create the message.
    StringBuffer messageBuffer = new StringBuffer();
    int size = mListCanvas.size();
    for (int i = 0; i < size; i++) {
      if (mListCanvas.isMarked(i)) {
        messageBuffer.append(mListCanvas.getShortText(i));
        messageBuffer.append('\n');
        messageBuffer.append(mListCanvas.getExtra(i));
        messageBuffer.append('\n');
        messageBuffer.append('\n');
      }
    }
    String message = messageBuffer.toString();
    
    MailTask task = new MailTask(url, log, email, message);
    mWorker.queue(task);
  }

  private void createQueryForm(Vector parameters) {
    if (mQueryForm == null) {
      mQueryForm = new QueryForm("Entry");
      mSubmitCommand = new Command("Submit", Command.OK, 0);
      mQueryForm.addCommand(mSubmitCommand);
      mQueryForm.addCommand(mBackCommand);
      mQueryForm.setCommandListener(this);
    }
    mQueryForm.setParameters(parameters);
  }
  
  private Form getPreferencesEditor() {
    if (mPreferencesEditor == null) {
      boolean valid = validate();
      // Create the preferences editor.
      String[] noShows = { "PeekAndPick Server" };
      mPreferencesEditor = new PreferencesEditor(noShows);
      mPreferencesEditor.setType("Email address",
          PreferencesEditor.kText,
          TextField.EMAILADDR);
      mPreferencesEditor.setType("Font size",
          PreferencesEditor.kChoice,
          new String[] { "Small", "Medium", "Large" });
      mPreferencesEditor.initialize(mPreferences);
      // Only allow Back if the email address is valid.
      if (valid == true)
        mPreferencesEditor.addCommand(mBackCommand);
      mPreferencesEditor.addCommand(mSaveCommand);
      mPreferencesEditor.setCommandListener(this);
    }
    return mPreferencesEditor;
  }
  
  private boolean validate() {
    return validate(mPreferences.getHashtable());
  }
  
  private boolean validate(Hashtable hashtable) {
    String email = (String)hashtable.get("Email address");
    
    boolean valid = true;
    if (email == null) valid = false;
    else if (email.length() == 0) valid = false;
    else {
      int atSign = email.indexOf('@');
      if (atSign <= 0) valid = false;
      else if (atSign >= email.length() - 1) valid = false;
    }
    
    return valid;
  }
}

