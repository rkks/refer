package util;

/**
 * <code>WorkerTask</code> is implemented by tasks that can be run
 * by a <code>Worker</code>.
 *
 * @author Jonathan Knudsen
 */
public interface WorkerTask {
  /**
   * This method is called from within the
   * <code>Worker</code>'s thread. Note that this method,
   * in constrast to <code>java.lang.Runnable</code>'s
   * <code>run()</code>method, may throw an exception.
   */
  public void run() throws Exception;
  
  /**
   * This method may be called at any time. It is the task's
   * responsibility to terminate gracefully.
   */
  public void cancel();
}
