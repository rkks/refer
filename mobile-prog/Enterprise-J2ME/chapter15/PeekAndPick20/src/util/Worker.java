package util;

import java.util.Vector;

import display.ProgressDisplay;

/**
 * <code>Worker</code> is a class that can run tasks inside a separate,
 * dedicated thread.
 * The new thread is created and run inside <code>Worker</code>'s
 * constructor. You can request that a task be run by
 * passing a <code>WorkerTask</code> to the <code>queue()</code> method.
 *
 * @author Jonathan Knudsen
 */
public class Worker
    implements Runnable {
  private boolean mTrucking;
  private Vector mQueue;
  private WorkerListener mWorkerListener;
  private boolean mCancel;
  private ProgressDisplay mProgressDisplay;
  
  /**
   * Creates a new <code>Worker</code> and kicks off its thread.
   *
   * @param pd a <code>ProgressDisplay</code> implementation
   *     that will show an indicator while this
   *     <code>Worker</code> is performing a task
   */
  public Worker(ProgressDisplay pd) {
    mTrucking = true;
    mQueue = new Vector();
    mCancel = false;
    mProgressDisplay = pd;
    
    Thread t = new Thread(this);
    t.start();
  }
  
  /**
   * Assigns a listener object.
   */
  public void setWorkerListener(WorkerListener listener) {
    mWorkerListener = listener;
  }
  
  /**
   * Queues a <code>WorkerTask</code> for execution. The
   * task executes immediately if the queue was empty.
   */
  public void queue(WorkerTask r) {
    // Add it to the queue.
    mQueue.addElement(r);
    
    // Unstick run(). Only call this if we've just added
    // a task to an empty queue, which means that run()
    // is waiting to be woken up.
    if (mQueue.size() == 1)
      synchronized(this) { notify(); }
  }
  
  /**
   * This method is called by the constructor. Normally
   * you will not need to call this method yourself.
   */
  public synchronized void run() {
    while (mTrucking) {
      // If there are no more tasks in the queue,
      // go to sleep.
      if (mQueue.size() == 0) {
        mProgressDisplay.setProgress(false);
        try { wait(); }
        catch (InterruptedException ie) {}
      }
      // If we're still running.
      if (mTrucking) {
        WorkerTask r = (WorkerTask)mQueue.elementAt(0);
        try {
          mProgressDisplay.setProgress(true);
          r.run();
          mWorkerListener.finished(r);
        }
        catch (Exception e) {
          if (mCancel == false)
            mWorkerListener.exception(r, e);
          mCancel = false;
        }
        // Whatever happened, remove the task from the
        // queue.
        mQueue.removeElementAt(0);
      }
    }
  }
  
  /**
   * Cancels the currently running task.
   */
  public void cancel() {
    WorkerTask task = null;
    try { task = (WorkerTask)mQueue.elementAt(0); }
    catch (ArrayIndexOutOfBoundsException aioobe) {}
    if (task == null) return;
    mCancel = true;
    task.cancel();
  }
  
  /**
   * Shuts down this <code>Worker</code>.
   */
  public void stop() {
    mTrucking = false;
    notify();
  }
}
