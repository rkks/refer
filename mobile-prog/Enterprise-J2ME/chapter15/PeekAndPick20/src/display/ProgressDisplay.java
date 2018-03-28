package display;

/**
 * This is a simple interface for an object that is
 * capable of showing a visual indicator of work.
 */
public interface ProgressDisplay {
  /**
   * Use this method to turn the progress indicator on
   * or off.
   */
  public void setProgress(boolean b);
}
