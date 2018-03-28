package display;

import java.io.IOException;

import javax.microedition.lcdui.*;

/**
 * A simple splash screen.
 *
 * @author Jonathan Knudsen
 * @since PeekAndPick 1.1
 */
public class SplashScreen
    extends Canvas
    implements Runnable {
  private Image mImage;
  private Display mDisplay;
  private Displayable mNextDisplayable;
  
  /**
   * Create a new <code>SplashScreen</code>. The constructor
   * attempts to load the named image and begins a timeout
   * thread. The splash screen can be dismissed with a key
   * press, a pointer press, or a timeout (hardcoded at
   * three seconds).
   *
   * @param name the path of the image resource to load
   * @param display the application's <code>Display</code>
   * @param next the screen to be shown after the splash
   */
  public SplashScreen(String name,
      Display display, Displayable next) throws IOException {
    mImage = Image.createImage(name);
    mDisplay = display;
    mNextDisplayable = next;
    Thread t = new Thread(this);
    t.start();
  }
  
  /**
   * Paints the image centered on the screen.
   */
  public void paint(Graphics g) {
    int width = getWidth();
    int height = getHeight();

    g.drawImage(mImage, width / 2, height / 2,
        Graphics.HCENTER | Graphics.VCENTER);
  }
  
  /**
   * Dismisses the splash screen.
   */
  public void dismiss() {
    if (isShown())
      mDisplay.setCurrent(mNextDisplayable);
  }
  
  /**
   * This method is used internally with a timeout thread.
   */
  public void run() {
    try { Thread.sleep(3000); }
    catch (InterruptedException ie) {}
    dismiss();
  }
  
  /**
   * A key release event causes the <code>dismiss()</code>
   * method to be called.
   */
  public void keyReleased(int keyCode) { dismiss(); }
  /**
   * A pointer release event causes the <code>dismiss()</code>
   * method to be called.
   */
  public void pointerReleased(int x, int y) { dismiss(); }
}
