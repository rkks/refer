import java.util.ArrayList;
import java.util.StringTokenizer;


public class MyChecker implements Checker {

  // a sequence of application events

  private ArrayList seq;



  public MyChecker () {

  }
  
  private boolean isNeighbor(int i, int j) {
     if ((i==(j+DistributedDiningPhilosophers.numberOfProcesses-1)%DistributedDiningPhilosophers.numberOfProcesses) 
     || (i==(j+1)%DistributedDiningPhilosophers.numberOfProcesses))
     	  return true;
     else 
        return false;
  }

  public boolean check (ArrayList seq) {
  
 	 //System.out.println("*****");
	 //for (int i=0; i<seq.size();i++)
	 //	System.out.println((AppEvent)seq.get(i));
	 //System.out.println("*****");
	 
    boolean rval = true;

    // check for mutual exclusion

    for (int i = 0; i < seq.size (); i ++) {

      AppEvent event = (AppEvent) seq.get(i);

      for (int j = 0; j < seq.size (); j ++) {

	if (i != j) {

	  AppEvent it = (AppEvent) seq.get(j);
	  String eventLabel = event.getLabel();
	  String itLabel = it.getLabel();
	  StringTokenizer eventTokenizer = new StringTokenizer(eventLabel," ");
	  StringTokenizer itTokenizer = new StringTokenizer(itLabel," ");	 
	  int eventID = Integer.parseInt(eventTokenizer.nextToken());;
	  int itID = Integer.parseInt(itTokenizer.nextToken());;
	  if (isNeighbor (eventID, itID)

	      && event.isConcurrent (it)) {

	    rval = false;

	    break;

	  }

	}

      }

      

      if (rval) {

			//System.out.println("Success");

      }

      else {

			System.out.println("Failure: Two neighboring diners are eating.");

      }

    }

    

    return rval;

  }

}

