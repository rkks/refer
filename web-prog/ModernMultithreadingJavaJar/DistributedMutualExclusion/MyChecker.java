import java.util.ArrayList;

public class MyChecker implements Checker {
  // application thread IDs
  private int[] tids;
  // a sequence of application events
  private ArrayList seq;
  public MyChecker () {
    //this.tids = tids;
    // hardcode thread IDs for now
    tids = new int [3];
    tids[0] = 1; tids[1] = 4; tids[2] = 7;
  }
  public boolean check (ArrayList seq) {
 	 //System.out.println("*****");
	 //for (int i=0; i<seq.size();i++)
	 //	System.out.println((AppEvent)seq.get(i));
	 //System.out.println("*****");
    boolean rval = true;
    // check for eventual entry
    int culprit = -1;
    for (int i = 0; i < tids.length; i ++) {
      boolean entered = false;
      for (int j = 0; j < seq.size (); j ++) {
      	AppEvent event = (AppEvent) seq.get(j);
			if (tids[i] == event.getThreadID ()) {
			  entered = true;
			  break;
			}
      }
      if (!entered) {
			culprit = tids[i];
			rval = false;
			break;
      }
    }
    if (!rval) {
      System.out.println("Failed: Thread " + culprit + " did not enter.");
    }
    else {
      // check for mutual exclusion
      for (int i = 0; i < seq.size (); i ++) {
			AppEvent event = (AppEvent) seq.get(i);
			for (int j = 0; j < seq.size (); j ++) {
			  if (i != j) {
			    AppEvent it = (AppEvent) seq.get(j);
			    if (event.isConcurrent (it)) {
		      	System.out.println("Concurrent application events:" +
				 		event + "; " + it); 
			      rval = false;
			      break;
			    }
		   }
		}
		if (!rval) {
		  System.out.println("Failed: Mutual exclusion violated.");
		}
		else {
		  //System.out.println("Passed");
		}
  	 }

	}
    return rval;
  }
}
