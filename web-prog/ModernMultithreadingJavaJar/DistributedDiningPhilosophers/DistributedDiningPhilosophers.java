public class DistributedDiningPhilosophers {
	static final int numberOfProcesses =3; // must be 2 or more
	public static void main(String[] args) {
		distributedPhilosopher DP[] = new distributedPhilosopher[numberOfProcesses];
		asynchPort[] messageChannels = new asynchPort[numberOfProcesses];
		
		for (int i = 0; i < numberOfProcesses; i++) {
			messageChannels[i] = new asynchPort("messageChannel"+i);
		}
		for (int i = 0; i < numberOfProcesses; i++)
			(DP[i] = new distributedPhilosopher(i,numberOfProcesses,messageChannels)).start();

		try {
			for (int i = 0; i < numberOfProcesses; i++)
				DP[i].join();
		}
		catch (InterruptedException e) {}
	}
}

class distributedPhilosopher extends TDThread {
	private int ID;									// process ID
	private int numberOfProcesses;
	private asynchPort[] messageChannels;		// messages sent to other philosophers
	private Coordinator C;							// monitor
	private Helper helper;							// manages incoming requests
	final int iterations = 1;

	distributedPhilosopher(int ID, int numberOfProcesses, asynchPort messageChannels[]) {
		super("distributedPhilosopher"+ID);
		this.ID = ID;
		this.numberOfProcesses = numberOfProcesses;
		this.messageChannels = messageChannels;
		C = new Coordinator();
		helper = new Helper("helper"+ID);
		messageChannels[ID].setOwner(helper);
		helper.setDaemon(true);
		helper.start();
	}

	class Helper extends TDThread {
	// manages requests from other distributed processes
		Helper(String name) {
			super(name);
		}
		public void run() { // handle requests from other distributedProcesses
			int messageCount = 0;
			int ateMessages = 0;
			if (numberOfProcesses<3) // numberOfProcesses must be 2 or more
			  ateMessages = 2;
			else
			  ateMessages=3;
			while (true) {
				Message msg = (Message) messageChannels[ID].receive();
				if (msg.tag.equals("Ate"))
					messageCount++;	
				else 
					C.handleMessage(msg);
				if (messageCount == ateMessages*iterations) { // get message from self and neighbors
					break;
				}
			}
			return;
		}
	}
	
   private boolean isNeighbor(int i) {
     if ((i==(ID+numberOfProcesses-1)%numberOfProcesses) || (i==(ID+1)%numberOfProcesses))
     	  return true;
     else 
	        return false;
   }

	class Coordinator extends monitorSU {
	// Synchronizes the distributed process and its helper
	   private static final int thinking = 0, hungry = 1, eating = 2;
	   private boolean fork[] = null,  dirty[] = null, request[] = null;
	   private int myState = thinking;
	   private conditionVariable myForks;
		public Coordinator() {
			super("Coordinator" + ID);
			myForks = new conditionVariable("myForks"+ID);
	      fork = new boolean[numberOfProcesses]; dirty = new boolean[numberOfProcesses];
	      request = new boolean[numberOfProcesses];
	      for (int i = 0; i < numberOfProcesses; i++) {
	         if ((ID > i) && (isNeighbor(i))) {
	            fork[i] = false; request[i] = true;
	         } 
	         else { 
	         	fork[i] = true; request[i] = false; 
	         }
   	      dirty[i] = true;
	       }
	   }
		public void handleMessage(Message msg) {
			enterMonitor("handleMessage");
	      if (msg.tag.equals("Request")) {
	         request[msg.src] = true;
	         if ((myState != eating) && fork[msg.src] && dirty[msg.src]) {
	                messageChannels[msg.src].send(new Message(ID,msg.src,"Fork")); 
	                fork[msg.src] = false;
		                if (myState == hungry){
	                    messageChannels[msg.src].send(new Message(ID,msg.src,"Request")); 
	                    request[msg.src] = false;
	                }
	         }
	      } 
	      else if (msg.tag.equals("Fork")) {
	            fork[msg.src] = true; dirty[msg.src] = false;
	            if (haveForks()) {
	                myState = eating; myForks.signalC();
	            }
	      }		
			exitMonitor();
		}
		
		public void requestCS() {	
			enterMonitor("requestCS");
	      myState = hungry;
	      if (haveForks()) {
	      	myState = eating;
	     	}
	      else {
 	         for (int i = 0; i < numberOfProcesses; i++) {
	            if (request[i] && !fork[i]) {
	                messageChannels[i].send(new Message(ID,i,"Request"));
	                request[i] = false;
	            }
	         }
	      }
	      while (myState != eating) myForks.waitC();
			exitMonitor();
		}
	
		public void releaseCS() {
			enterMonitor("releaseCS");
	      myState = thinking;
	      for (int i = 0; i < numberOfProcesses; i++) {
	         dirty[i] = true;
	         if (request[i]) { 
	         	messageChannels[i].send(new Message(ID,i,"Fork"));
	         	fork[i] = false; 
	         }
	         if (isNeighbor(i) || i==ID) {
  	         	messageChannels[i].send(new Message(ID,i,"Ate"));
  	         }
	      }
			exitMonitor();
		}
	
	    private boolean haveForks() {
	       for (int i = 0; i < numberOfProcesses; i++) {
	         if (!fork[i]) 
	           	return false;
	       }
	       return true;
	    }
	
	}
	
	public void run() {
		int count = 0;		//System.out.println("Philosopher " + ID + " running");
		while (count++<iterations) {
			C.requestCS();
			ApplicationEvents.exerciseEvent(ID + " eats");
			C.releaseCS();
			try {
				helper.join();
			}
			catch (InterruptedException e) {}
		}
		return;
	}
}
	
	class Message {
		public int src;
		public int destination;
		public String tag;
		public Message(int src, int destination, String tag) {
			this.src = src; this.destination = destination; this.tag = tag;
		}
	}
