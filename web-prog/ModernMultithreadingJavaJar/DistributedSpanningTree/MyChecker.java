import java.util.ArrayList;
import java.util.StringTokenizer;


public class MyChecker implements Checker {

  // a sequence of application events

  private ArrayList seq;

  public MyChecker () {

  }
  
  public boolean check (ArrayList seq) {

 	 //System.out.println("*****");
	 //for (int i=0; i<seq.size();i++)
	 //	System.out.println((AppEvent)seq.get(i));
	 //System.out.println("*****");
	 
    boolean rval = true;
    
    AppEvent event = (AppEvent) seq.get(0);
	 
    neighborMatrix m = (neighborMatrix) event.getObject();
    
    boolean[][] matrix = m.getNeighbors ();
    
    
    // example: for 3 nodes: 
    // FTT       : for node 0, there is an edge from 0 to 1 and 0 to 2
    // TFT       : for node 1, there is an edge from 1 to 0 and 1 to 2
    // TTF       : for node 2, there is an edge from 2 to 0 and 2 to 1
		
    //      0
    //     / \
    //    1---2
	 
      //System.out.println(m);
      
      // m[i][j] is true if there is an edge between i and j
	 
      event = (AppEvent) seq.get(1);
		
      childrenArray c = (childrenArray) event.getObject();
	 
      //System.out.println(c);
		
      // c[i] is j if there is an edge from i to j in the tree
      // c[0] = 0 as node 0 is always the root of the tree
      // example: [0,2,0] means:
      // 0 is the root
      // there is an edge from 1 to 2
      // there is an edge from 2 to 0
      //
      //       0
      //        \
	//      1--2
	//
	// c[i] is initalized to -1 for all i
	// if c[i] is j then m[i,j] should be true

	int[] children = c.getChildren ();

	// check every edge is in the original graph
	// skip node 0
	for (int i = 1; i < children.length; i ++) {
	  if (children[i] == -1
	      || matrix[i][children[i]] == false) {
	    rval = false;
	    break;
	  }
	}
	
	if (rval) {
	  for (int i = 0; i < children.length; i ++) {
	    int steps = children.length;
	    int parent = children[i];
	    while (steps > 0 && parent != 0) {
	      parent = children[parent];
	      steps --;
	    }
	    if(parent != 0) {
	      rval = false;
	      break;
	    }
	  }
	}
		  
	if (rval) {
	  //System.out.println("Success");
	}
	else {
	  System.out.println("Failure: ... ");
	}
		  
	return rval;
  }
}

