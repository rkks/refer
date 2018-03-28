package display;

import java.util.Vector;

import javax.microedition.lcdui.*;

/**
 * ItemCanvas shows the short text of a DisplayItem as
 * a header, and the long text as a scrolling area below.
 */
class ItemCanvas
    extends Canvas
    implements CommandListener {
  private Display mDisplay;
  private Displayable mPreviousDisplayable;
  
  // General information.
  private int mWidth, mHeight;
  private int mHeaderHeight, mFooterHeight;
  private Font mFont;
  private int mLineHeight;
  
  // Variables for paging.
  private int mScroll, mLastVisible, mPageSize;
  
  // The data we display.
  private Vector mWrappedTitle, mWrappedBody;
  
  public ItemCanvas(Display display, Displayable previous,
      Font font, DisplayItem item) {
    mDisplay = display;
    mPreviousDisplayable = previous;
    
    mHeaderHeight = ListCanvas.kUpImage.getHeight();
    mFooterHeight = ListCanvas.kDownImage.getHeight();

    mWidth = getWidth();
    mHeight = getHeight();
    mFont = font;
    mLineHeight = mFont.getHeight();

    wrap(item);
    mScroll = 0;
    
    Command back = new Command("Back", Command.BACK, 0);
    addCommand(back);
    setCommandListener(this);
   }
  
  public void paint(Graphics g) {
    // Clear the screen.
    g.setColor(255, 255, 255);
    g.fillRect(0, 0, mWidth, mHeight);
    
    // Set defaults.
    g.setColor(0, 0, 0);
    g.setFont(mFont);
    
    // Draw the header.
    paintHeader(g);
    
    // Draw as many lines as will fit between the header and the footer.
    paintBody(g);
    
    // Draw footer.
    paintFooter(g);
  }
  
  public void commandAction(Command c, Displayable s) {
    back();
  }
  
  private void back() {
    mDisplay.setCurrent(mPreviousDisplayable);
  }
    
  private void paintHeader(Graphics g) {
    // Draw the title.
    int x = ListCanvas.kLeftImage.getWidth();
    int y = 0;
    boolean trucking = true;
    int index = 0;
    
    g.drawImage(ListCanvas.kLeftImage, 0, mLineHeight / 2,
        Graphics.VCENTER | Graphics.LEFT);
    
    while (trucking) {
      if (index >= mWrappedTitle.size()) trucking = false;
      else {
        String line = (String)mWrappedTitle.elementAt(index);
        g.drawString(line, x, y, Graphics.TOP | Graphics.LEFT);

        index++;
        y += mLineHeight;
        x = 0;
      }
    }
    
    // Draw a separator line.
    g.setStrokeStyle(Graphics.DOTTED);
    g.drawLine(0, y, mWidth, y);
    g.setStrokeStyle(Graphics.SOLID);

    if (mScroll != 0) {
      g.drawImage(ListCanvas.kUpImage, mWidth / 2, y + 1,
          Graphics.HCENTER | Graphics.TOP);
    }
    
    // Set mHeaderHeight appropriately.
    mHeaderHeight = y + ListCanvas.kUpImage.getHeight() + 1;
  }
  
  private void paintBody(Graphics g) {
    // paint the text of the description into the available space
    int y = mHeaderHeight;
    boolean trucking = true;
    int index = mScroll;
    while (trucking) {
      if (index >= mWrappedBody.size()) trucking = false;
      else if (y + mLineHeight > (mHeight - mFooterHeight)) trucking = false;
      else {
        // Draw the line.
        String line = (String)mWrappedBody.elementAt(index);
        g.drawString(line, 0, y, Graphics.TOP | Graphics.LEFT);

        index++;
        y += mLineHeight;
      }
    }
    
    // Set the last visible item index.
    mLastVisible = index - 1;
  }
  
  private void wrap(DisplayItem item) {
    if (mWrappedTitle == null) mWrappedTitle = new Vector();
    if (mWrappedBody == null) mWrappedBody = new Vector();
    
    wrap(item.getShortText(), mWrappedTitle, true);
    wrap(item.getLongText(), mWrappedBody, false);
  }
  
  private void wrap(String s, Vector lines, boolean hasImage) {
    if (s == null) return;
    
    boolean trucking = true;
    String word;
    int index = 0, lastSpace = -1;
    while (trucking) {
      // Find the next space.
      int end = s.indexOf(' ', lastSpace + 1);
      if (end == -1)
        end = s.length() - 1;
      
      // Measure the string width.
      int lineWidth = mFont.substringWidth(s, index, end - index);
      
      // Compute image width;
      int imageWidth = 0;
      if (index == 0 && hasImage == true)
        imageWidth = ListCanvas.kLeftImage.getWidth();
      
      // Put a line away if it's too wide.
      if (lineWidth > (mWidth - imageWidth)) {
        String line;
        if (lastSpace <= index) {
          line = s.substring(index, end);
          index = end + 1;
        }
        else if (lastSpace == -1) {
          line = s.substring(index);
          trucking = false;
        }
        else {
          line = s.substring(index, lastSpace);
          index = lastSpace + 1;
        }
        lines.addElement(line);
      }
      lastSpace = end;
      
      // Exit if we're done.
      if (end == s.length() - 1) {
        String line = s.substring(index);
        lines.addElement(line);
        trucking = false;
      }
    }
  }
  
  private void paintFooter(Graphics g) {
    if (mLastVisible >= (mWrappedBody.size() - 1)) return;
    g.drawImage(ListCanvas.kDownImage,
        mWidth / 2, mHeight - mFooterHeight,
        Graphics.HCENTER | Graphics.TOP);
  }
  
  protected void keyPressed(int keyCode) {
    int gameAction = getGameAction(keyCode);
    switch(gameAction) {
      case UP:
        if (mScroll > 0) {
          mScroll -= mPageSize;
          if (mScroll < 0) mScroll = 0;
          repaint();
        }
        break;
      case DOWN:
        if (mLastVisible < (mWrappedBody.size() - 1)) {
          mScroll = mLastVisible + 1;
          if (mPageSize == 0) mPageSize = mLastVisible + 1;
          repaint();
        }
        break;
      case RIGHT:
        break;
      case LEFT:
        back();
        break;
    }
  }
}

