package display;

import java.util.Vector;

import javax.microedition.lcdui.*;

/**
 * <p>
 * <code>ListCanvas</code> represents a scrollable display of
 * items. Items consist of short text, which is shown
 * on this screen, long text, which is shown on a detail screen,
 * and extra text, which is not shown but is accessible.
 * <code>ListCanvas</code> supports scrolling up and down. Pressing
 * on the right arrow displays a detail screen for the
 * selected item. Items in the <code>ListCanvas</code> can also be
 * marked by pressing on the fire button.
 * </p>
 * <p>
 * As of PeekAndPick 2.0, <code>ListCanvas</code> also
 * supports the <code>ProgressDisplay</code> interface
 * with a simple animated progress indicator in the footer.
 * </p>
 *
 * @see display.ProgressDisplay
 * @author Jonathan Knudsen
 */
public class ListCanvas
    extends Canvas
    implements Runnable, ProgressDisplay {
  private Display mDisplay;
  
  // The data we display.
  private Vector mItems, mMarks;
  private int mSelection;
  
  // Helpers for marking.
  private boolean mMarkingAllowed = true;
  private static final Boolean kFalse = new Boolean(false);
  private static final Boolean kTrue = new Boolean(true);
  
  // General information.
  private int mWidth, mHeight;
  private int mHeaderHeight, mFooterHeight;
  private Font mFont;
  private int mLineHeight;
  private int mLeftMargin;
  
  // Cancel command.
  private Command mCancelCommand;
  
  // For progress display.
  private int mProgress;
  
  // Variables for paging.
  private int mScroll, mLastVisible, mPageSize;
  
  // Useful images.
  protected static Image kSelectionImage,
      kUpImage, kDownImage, kLeftImage, kRightImage,
      kMarkImage, kProgressImage;
  
  private static void loadImages() {
    try {
      kSelectionImage = Image.createImage("/selection.png");
      kUpImage = Image.createImage("/up.png");
      kDownImage = Image.createImage("/down.png");
      kLeftImage = Image.createImage("/left.png");
      kRightImage = Image.createImage("/right.png");
      kMarkImage = Image.createImage("/mark.png");
      kProgressImage = Image.createImage("/progress.png");
    }
    catch (java.io.IOException ioe) { System.out.println(ioe); }
  }
  
  /**
   * Creates a new ListCanvas.
   *
   * @param display the application's <code>Display</code>
   * @param cancel a cancel command that is shown at the
   *     same time as the progress indicator
   */
  public ListCanvas(Display display, Command cancel) {
    if (kSelectionImage == null) loadImages();
    mDisplay = display;
    mCancelCommand = cancel;

    mHeaderHeight = kUpImage.getHeight();
    mFooterHeight = kDownImage.getHeight();
    mLeftMargin = kSelectionImage.getWidth() + kMarkImage.getWidth();

    mWidth = getWidth();
    mHeight = getHeight();
    setFontSize("Small");

    clear();
    
    Thread t = new Thread(this);
    t.start();
  }
  
  /**
   * Determines whether marking is allowed.
   */
  public void setMarkingAllowed(boolean markingAllowed) {
    mMarkingAllowed = markingAllowed;
  }
  
  /**
   * returns the number of items in this list.
   */
  public int size() { return mItems.size(); }
  
  /**
   * Removes all items from the list.
   */
  public void clear() {
    mItems = new Vector();
    mMarks = new Vector();
    mSelection = 0;
    mScroll = 0;
  }
  
  /**
   * Sets the font size that is used to display items.
   *
   * @param size valid values are &quot;Small&quot;,
   *     &quot;Medium&quot;, and &quot;Large&quot;
   */
  public void setFontSize(String size) {
    int s = Font.SIZE_SMALL;
    if (size.equals("Small")) s = Font.SIZE_SMALL;
    else if (size.equals("Medium")) s = Font.SIZE_MEDIUM;
    else if (size.equals("Large")) s = Font.SIZE_LARGE;
    
    mFont = Font.getFont(Font.FACE_PROPORTIONAL,
        Font.STYLE_PLAIN, s);
    mLineHeight = mFont.getHeight();
  }
  
  private DisplayItem getItem(int i) {
    return (DisplayItem)mItems.elementAt(i);
  }
  
  /**
   * Returns the short text for the item at the specified
   * index.
   */
  public String getShortText(int i) {
    DisplayItem item = (DisplayItem)mItems.elementAt(i);
    return item.getShortText();
  }

  /**
   * Returns the long text for the item at the specified
   * index.
   */
  public String getLongText(int i) {
    DisplayItem item = (DisplayItem)mItems.elementAt(i);
    return item.getLongText();
  }

  /**
   * Returns the extra text for the item at the specified
   * index.
   */
  public String getExtra(int i) {
    DisplayItem item = (DisplayItem)mItems.elementAt(i);
    return item.getExtra();
  }

  /**
   * Tells whether the item at the specified index has
   * been marked.
   */
  public boolean isMarked(int i) {
    return (mMarks.elementAt(i) == kTrue);
  }

  /**
   * Returns true if any of the items have been
   * marked.
   */
  public boolean hasMarks() {
    boolean hasMarks = false;
    for (int i = 0; i < size(); i++) {
      if (isMarked(i) == true) {
        hasMarks = true;
        break;
      }
    }
    return hasMarks;
  }

  /**
   * Adds a new item to the list.
   *
   * @param shortText the short text for the new item
   * @param longText the long text for the new item
   * @param extra the extra text for the new item
   */
  public void addItem(String shortText, String longText,
      String extra) {
    DisplayItem item = new DisplayItem(shortText, longText,
        extra);
    mItems.addElement(item);
    mMarks.addElement(kFalse); // Default to unmarked.
    repaint();
  }
  
  /**
   * Shows the details screen for the current item.
   */
  public void details() {
    DisplayItem item = (DisplayItem)mItems.elementAt(mSelection);
    ItemCanvas itemCanvas = new ItemCanvas(mDisplay, this, mFont, item);
    mDisplay.setCurrent(itemCanvas);
  }
  
  /**
   * Toggles the mark for the current item.
   */
  public void mark() {
    if (mMarkingAllowed == true) {
      if (mMarks.elementAt(mSelection) == kFalse)
        mMarks.setElementAt(kTrue, mSelection);
      else
        mMarks.setElementAt(kFalse, mSelection);
      repaint();
    }
  }
  
  /**
   * Toggles the display of the progress bar and
   * the presence of the <b>Cancel</b> command.
   *
   * @param b If <code>b</code> is <code>true</code>, the
   *     progress bar is shown and the cancel command is
   *     added. Otherwise, the progress bar is removed
   *     and the cancel command is removed.
   */
  public void setProgress(boolean b) {
    if (b == true) {
      mProgress = 1;
      addCommand(mCancelCommand);
    }
    else {
      mProgress = 0;
      removeCommand(mCancelCommand);
      repaint();
    }
  }
  
  /**
   * Called internally. Runs a loop to animate the
   * progress indicator.
   */
  public void run() {
    while (true) {
      try { Thread.sleep(125); }
      catch (InterruptedException ie) {}
      if (mProgress != 0) {
        mProgress++;
        repaint();
      }
    }
  }
  
  /**
   * Paints the item list.
   */
  public void paint(Graphics g) {
    // Clear the screen.
    g.setColor(255, 255, 255);
    g.fillRect(0, 0, mWidth, mHeight);
    
    // Set defaults.
    g.setColor(0, 0, 0);
    g.setFont(mFont);
    
    // Draw the header if appropriate.
    paintHeader(g);
    
    // Draw as many items as will fit between the header and the footer.
    paintBody(g);
    
    // Draw footer.
    paintFooter(g);
  }
  
  private void paintHeader(Graphics g) {
    if (mScroll != 0)
      g.drawImage(kUpImage, mWidth / 2, 0,
          Graphics.HCENTER | Graphics.TOP);
  }
  
  private void paintBody(Graphics g) {
    int y = mHeaderHeight;
    boolean trucking = true;
    int index = mScroll;
    while (trucking) {
      if (index >= mItems.size()) trucking = false;
      else if (y + mLineHeight >= (mHeight - mFooterHeight)) trucking = false;
      else {
        // Draw the item.
        DisplayItem item = (DisplayItem)mItems.elementAt(index);
        String shortText = item.getShortText();
        g.drawString(shortText, mLeftMargin, y,
            Graphics.TOP | Graphics.LEFT);
        // If it's too wide, or if there is long text
        // available, show a right arrow.
        if (mFont.stringWidth(shortText) >
            (mWidth - mLeftMargin) ||
            item.getLongText() != null) {
          // Clear the area.
          int oldColor = g.getColor();
          g.setColor(255, 255, 255);
          g.fillRect(mWidth - kRightImage.getWidth(), y,
              kRightImage.getWidth(), mLineHeight);
          g.setColor(oldColor);
          // Draw the right arrow.
          g.drawImage(kRightImage, mWidth, y + mLineHeight / 2,
              Graphics.VCENTER | Graphics.RIGHT);
        }
        
        // Draw the mark.
        if (mMarks.elementAt(index) == kTrue)
          g.drawImage(kMarkImage, mLeftMargin, y + mLineHeight / 2,
              Graphics.VCENTER  | Graphics.RIGHT);

        // Draw the highlight.
        if (index == mSelection)
          g.drawImage(kSelectionImage, 0, y + mLineHeight / 2,
              Graphics.VCENTER  | Graphics.LEFT);
        
        index++;
        y += mLineHeight;
      }
    }
    
    // Set the last visible item index.
    mLastVisible = index - 1;
  }
  
  private void paintFooter(Graphics g) {
    // Determine the dimensions of the progress indicator.
    int w = mWidth - 8;
    int h = mFooterHeight;
    int x = 4;
    int y = mHeight - h;
    // Find out the width of the image, and tile if
    // necessary.
    int pw = kProgressImage.getWidth();
    if (mProgress != 0) {
      // Draw the framing rectangle.
      g.drawRect(x, y, w - 1, h - 1);
      // Save the old clip.
      int cx = g.getClipX();
      int cy = g.getClipY();
      int cw = g.getClipWidth();
      int ch = g.getClipHeight();
      // Set the clip.
      g.setClip(x + 1, y + 1, w - 2, h - 2);
      // Calculate the animation offset.
      int offset = -7 + mProgress % 8;
      // Tile the image.
      for (int ix = 0; ix + offset < w; ix += pw) { 
        g.drawImage(kProgressImage,
          x + ix + offset, y,
          Graphics.LEFT | Graphics.TOP);
      }
      // Restore the clip.
      g.setClip(cx, cy, cw, ch);
    }
    
    // Draw down arrow.
    if (mLastVisible < (mItems.size() - 1)) {
      g.drawImage(kDownImage, mWidth / 2, mHeight - mFooterHeight,
          Graphics.HCENTER | Graphics.TOP);
    }
  }
  
  /**
   * Responds to key presses.
   */
  protected void keyPressed(int keyCode) {
    int gameAction = getGameAction(keyCode);
    switch(gameAction) {
      case UP:
        if (mSelection > 0) {
          mSelection--;
          // Page up if necessary.
          if (mSelection < mScroll) {
            mScroll -= mPageSize;
            if (mScroll < 0) mScroll = 0;
          }
          repaint();
        }
        break;
      case DOWN:
        if (mSelection < (mItems.size() - 1)) {
          mSelection++;
          // Page down if necessary.
          if (mSelection > mLastVisible) {
            if (mPageSize == 0) mPageSize = mLastVisible + 1;
            mScroll = mSelection;
          }
          repaint();
        }
        break;
      case RIGHT:
        details();
        break;
      case FIRE:
        mark();
        break;
    }
  }
}

