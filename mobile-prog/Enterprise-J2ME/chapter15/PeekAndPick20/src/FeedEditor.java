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
 * <code>FeedEditor</code> is a <code>MIDlet</code> that
 * handles editing the feed list that is used by
 * PeekAndPick.
 *
 * @author Jonathan Knudsen
 * @see PeekAndPick
 * @see FeedStore
 * @since PeekAndPick 2.0
 */
public class FeedEditor
    extends MIDlet
    implements CommandListener {
  private static final String kBanner = "PeekAndPick Feed Editor";
  
  private FeedStore mFeedStore;
  
  private Display mDisplay;
  private Displayable mLastScreen;
  private Command mExitCommand, mBackCommand, mSaveCommand;
  private Command mAddCommand, mEditCommand, mDeleteCommand, mDefaultsCommand;
  
  private List mFeedList;
  private Form mEditForm;
  private TextField mNameField, mURLField;
  
  private Feed mEditFeed;
  
  private boolean mInitialized;
  
  public FeedEditor() {
    mInitialized = false;
  }
  
  public void startApp() {
    if (mDisplay == null) mDisplay = Display.getDisplay(this);
    if (mInitialized == false) initialize();
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
    else if (c == mBackCommand) {
      if (mEditFeed != null) {
        mNameField.setString("");
        mURLField.setString("");
        mEditFeed = null;
      }
      mDisplay.setCurrent(mFeedList);
    }
    else if (c == mSaveCommand) {
      String name = mNameField.getString();
      String url = mURLField.getString();
      // Validate the entry first.
      if (validate(name, url) == false) {
        Alert a = new Alert("Please try again",
            "Please enter a valid name and URL.", null, null);
        a.setTimeout(Alert.FOREVER);
        mDisplay.setCurrent(a, mEditForm);
      }
      // Don't allow a duplicate name except for
      // the case of editing.
      else if ((mEditFeed == null && getFeed(name) != null) ||
          (mEditFeed != null && !getFeed(name).equals(mEditFeed))) {
        Alert a = new Alert("Please try again",
            "That name is already used", null, null);
        a.setTimeout(Alert.FOREVER);
        mDisplay.setCurrent(a, mEditForm);
      }
      else {
        Feed feed = new Feed(name, url);
        if (mEditFeed == null)
          mFeedStore.add(feed);
        // For an edit, remove the old feed object.
        else {
          mFeedStore.replace(mEditFeed, feed);
          mEditFeed = null;
        }
        // Blank out the edit form fields.
        mNameField.setString("");
        mURLField.setString("");
        // Show the updated feed list.
        mFeedStore.save();
        populateFeedList();
        mDisplay.setCurrent(mFeedList);
      }
    }
    else if (c == mAddCommand) {
      mDisplay.setCurrent(mEditForm);
    }
    else if (c == mEditCommand || c == List.SELECT_COMMAND) {
      Feed f = getSelectedFeed();
      Vector parameters = f.getParameters();
      if (parameters != null && parameters.size() > 0) {
        Alert a = new Alert("Can't edit that feed",
            "Sorry, PeekAndPick cannot " +
            "edit feeds with parameters.", null, null);
        a.setTimeout(Alert.FOREVER);
        mDisplay.setCurrent(a, mFeedList);
        return;
      }
      mEditFeed = f;
      mNameField.setString(mEditFeed.getName());
      mURLField.setString(mEditFeed.getURL());
      mDisplay.setCurrent(mEditForm);
    }
    else if (c == mDeleteCommand) {
      Feed f = getSelectedFeed();
      mFeedStore.remove(f);
      mFeedStore.save();
      populateFeedList();
      mDisplay.setCurrent(mFeedList);
    }
    else if (c == mDefaultsCommand) {
      mFeedStore.loadFeedsFromProperties("PeekAndPick.feed");
      mFeedStore.save();
      populateFeedList();
    }
  }
  
  // Private internal methods
  
  private void initialize() {
    // Pull the feeds out of persistent storage.
    mFeedStore = new FeedStore(
        "PeekAndPick.feeds", "PeekAndPick.feed", this);
    
    // Create commands.
    mExitCommand = new Command("Exit", Command.EXIT, 2);
    mAddCommand = new Command("Add", Command.SCREEN, 1);
    mEditCommand = new Command("Edit", Command.SCREEN, 1);
    mDeleteCommand = new Command("Delete", Command.SCREEN, 1);
    mBackCommand = new Command("Back", Command.BACK, 1);
    mSaveCommand = new Command("Save", Command.SCREEN, 1);
    mDefaultsCommand = new Command("Defaults", Command.SCREEN, 1);
    
    // Create the main screen, a list of feeds.
    mFeedList = new List(kBanner, List.IMPLICIT);
    populateFeedList();
    mFeedList.addCommand(mExitCommand);
    mFeedList.addCommand(mAddCommand);
    mFeedList.addCommand(mEditCommand);
    mFeedList.addCommand(mDeleteCommand);
    mFeedList.addCommand(mDefaultsCommand);
    mFeedList.setCommandListener(this);
    
    // Create the edit screen.
    mEditForm = new Form("Edit Feed");
    mNameField = new TextField("Name", "", 128, TextField.ANY);
    mURLField = new TextField("URL", "", 128, TextField.ANY);
    mEditForm.append(mNameField);
    mEditForm.append(mURLField);
    mEditForm.addCommand(mBackCommand);
    mEditForm.addCommand(mSaveCommand);
    mEditForm.setCommandListener(this);
    
    // Last screen is the feed list by default.
    mLastScreen = mFeedList;
    
    mInitialized = true;
  }

  private void populateFeedList() {
    // First remove all items from the list.
    while (mFeedList.size() > 0)
      mFeedList.delete(0);
    // Next walk through the FeedStore and show one item
    // for each Feed.
    for (int i = 0; i < mFeedStore.size(); i++) {
      Feed feed = mFeedStore.getFeed(i);
      mFeedList.append(feed.getName(), null);
    }
  }

  private Feed getSelectedFeed() {
    // First find the matching feed.
    String name = mFeedList.getString(
        mFeedList.getSelectedIndex());
    return getFeed(name);
  }
  
  private Feed getFeed(String name) {
    Feed feed = null;
    for (int i = 0; i < mFeedStore.size() && feed == null; i++) {
      feed = mFeedStore.getFeed(i);
      if (name.equals(feed.getName()) == false)
        feed = null;
    }
    return feed;
  }
  
  private boolean validate(String name, String url) {
    if (name == null) return false;
    if (url == null) return false;
    if (name.length() == 0) return false;
    if (url.length() == 0) return false;
    if (url.startsWith("http://") == false) return false;
    return true;
  }
}

