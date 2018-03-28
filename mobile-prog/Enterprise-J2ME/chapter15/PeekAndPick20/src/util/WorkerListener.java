package util;

/**
 * <code>WorkerListener</code> is the interface for objects that
 * receive notifications about <code>Worker</code> objects and
 * their tasks.
 *
 * @author Jonathan Knudsen
 */
public interface WorkerListener {
  /**
   * This method is called when an exception occurs within
   * a <code>WorkerTask</code>'s <code>run()</code> method.
   */
  public void exception(WorkerTask r, Exception e);
  /**
   * This method is called when the specified <code>WorkerTask</code>'s
   * <code>run()</code> method has finished.
   */
  public void finished(WorkerTask r);
}
