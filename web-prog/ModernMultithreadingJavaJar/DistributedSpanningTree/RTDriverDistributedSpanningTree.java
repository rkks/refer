public final class RTDriverDistributedSpanningTree implements propertyParameters {
// the driver for RT. Modify lines 16 and 34 to instantiate your program object
	private static int hash = 25; // progress displayed every hash executions, user can set this value
	private static int i = 1; // counts the number of executions
	private static int collected = 1;
	private static int numNonSystemThreads = 0;
	static RTWatchDog watchDog;
	static private propertyParameters.DetectDeadlock detectDeadlock = (propertyReader.getInstance().getDetectDeadlockProperty());
	static private propertyParameters.Mode mode = (propertyReader.getInstance().getModeProperty());
		public static void main (String args[]) {
		try {
		  	if (mode != RT) {
		  		System.out.println("Error: The RT Driver must be executed with mode=rt.");
		  		Exception e = new Exception(); StackTraceElement[] sTrace = e.getStackTrace();
		  		System.out.println("       For example: java -Dmode=rt " + sTrace[0].getClassName());
		  		System.exit(1);
		  	}
		  	msgTracingAndReplay.setUsingRTDriver();
			watchDog = new RTWatchDog(); watchDog.start();
  		   System.out.println("start:" + java.util.Calendar.getInstance().getTime());	   
  			// get a reference to the RT controller, which is a Singleton object
			//VariantGenerator generator = VariantGenerator.getInstance();
		   
			DST P = new DST(); 		
			P.main(args);
			// check whether any user-threads are still running. If so, stop RT	
			checkRunningThreads(); numNonSystemThreads=0;
			msgTracingAndReplay controller = channel.getObjectBasedController();
			boolean ignored = controller.appCompleted();
	
			if (ignored) System.out.println("Internal Error: !ignored failed");
			System.out.println("1/1");

			while (true) {
				ThreadIDGenerator.getInstance().resetIDs(); // reset ID generator
				channelIDGenerator.getInstance().resetIDs(); // reset ID generator
				conditionVarIDGenerator.getInstance().resetIDs();

				P = new DST();

				P.main(args); // execute P with input X using prefix-based testing
				// check whether any user-threads are still running. If so, stop RT
				checkRunningThreads(); numNonSystemThreads=0;

				controller = channel.getObjectBasedController();

				ignored = controller.appCompleted();
				if (!ignored)
					collected++;
				i++;
				if (i%hash==0)
				 System.out.println(i+"/"+collected);
			}
		} catch (Exception e) {e.printStackTrace();};
	}
	
	final static class RTWatchDog extends Thread {
		public RTWatchDog() {super("RTWatchDog");}
		public void run() {
			int delayTime = 8000;
		   RTStopWatch sw = new RTStopWatch(delayTime);
	      sw.start();  // capture start time
			while (true) {
				int saveIndex = i;
				try {
					Thread.sleep(delayTime);
				}
				catch (InterruptedException e) {}
				if (saveIndex == i) {
	    			sw.end();  // capture end time
	    			System.out.println();
	    			System.out.println("Reachability Testing completed.");
					System.out.println("  Executions:"+i+" / Sequences Collected:"+collected); // +"/"+transitionCount+"/"+eventCount);
	     			System.out.println("  Elapsed time in minutes: " + sw.elapsedMinutes());
	     			System.out.println("  Elapsed time in seconds: " + sw.elapsedSeconds());
	     			System.out.println("  Elapsed time in milliseconds: " + sw.elapsedMillis());
					System.out.flush();
					if (detectDeadlock == DETECTIONON) {
						deadlockWatchdog.stopWatchdog();
					}
					System.exit(0);
				}
			}
		}
	}

	static void checkRunningThreads() {
  		ThreadGroup root = Thread.currentThread().getThreadGroup().getParent();
  		while (root.getParent() != null) {
     		root = root.getParent();
  		}
    
  		// Visit each thread group
  		visit(root, 0);
	}
		
  	public static void visit(ThreadGroup group, int level) {
        // Get threads in `group'
			
		  if (group != null) {
	        int numThreads = group.activeCount();
      	  Thread[] threads = new Thread[numThreads*2];
	        numThreads = group.enumerate(threads, false);
	        for (int i=0; i<numThreads; i++) {
      	      // Get thread
   	         Thread thread = threads[i];
	            if (thread != null) {
	   	         ThreadGroup g = thread.getThreadGroup();
		            if (g != null) {
			            String groupName = g.getName();
			            if (groupName.equals("system"))
								;
		      	      else {
			            	if (!thread.getName().equals("RTWatchDog") && !thread.getName().equals("generatorThread")
			            		&& !thread.getName().equals("Controller") && !thread.getName().equals("deadlockWatchdog"))
				            	numNonSystemThreads++;
			   	         // The RTDriver has 4 associated threads that always run throughout RT
			      	      // (main thread, variant generator thread, controller thread, and RTWatchdog thread).
			         	   // If -DdeadlockDetection==on, then there is also a deadlock detection watchdog thread.
				            // Other threads should not be running when main returns. Here we try to check
				            // this and help the user.
	   		            if (numNonSystemThreads>1)  {
	   		            	System.out.println("");
		      	         	System.out.print("Error: Some of the threads in your program had ");
		         	      	System.out.println("not terminated when the main ");
		            	   	System.out.println("method returned to RTDriver. Use a try-catch block with a call to join()");
		               		System.out.print("for each thread ");
		               		System.out.println("to ensure that all threads are completed when main() returns.");
			               	System.out.println("For example:");
			               	System.out.println("     myThread t1 = new myThread();");
	   		            	System.out.println("     myThread t2 = new myThread();");
	      		         	System.out.println("     t1.start(); t2.start();");
	         		      	System.out.println("     try {");
	            		   	System.out.println("       t1.join();t2.join();");
	               			System.out.println("     } catch(InterruptedException e) {}");
	               			System.exit(1);
			               }
	   		         }
	   		       }
	   		    }
	        }
	    
	        int numGroups = group.activeGroupCount();
	        ThreadGroup[] groups = new ThreadGroup[numGroups*2];
	        numGroups = group.enumerate(groups, false);
    
	        // Recursively visit each subgroup
	        for (int i=0; i<numGroups; i++) {
	            visit(groups[i], level+1);
	        }
	     }
	}
}



