public class DistributedMutualExclusion {
	public static void main(String[] args) {
		final int numberOfProcesses = 3;
		distributedProcess DP[] = new distributedProcess[numberOfProcesses];
		asynchPort[] requestChannels = new asynchPort[numberOfProcesses];
		asynchPort[] replyChannels = new asynchPort[numberOfProcesses];
		
		for (int i = 0; i < numberOfProcesses; i++) {
			requestChannels[i] = new asynchPort("requestChannel"+i);
			replyChannels[i] = new asynchPort("replyChannel"+i);
		}

		for (int i = 0; i < numberOfProcesses; i++)
			(DP[i] = new distributedProcess(i,numberOfProcesses,requestChannels,replyChannels)).start();

		try {
			for (int i = 0; i < numberOfProcesses; i++)
				DP[i].join();
		}
		catch (InterruptedException e) {}
	}
}

class distributedProcess extends TDThread {
	private int ID;									// process ID

	private int replyCount;							// replies received so far
	private int numberOfProcesses;
	private asynchPort[] requestChannels;		// requests sent to other processes
	private asynchPort[] replyChannels;			// replies received in response to requests
	private Coordinator C;							// monitor
	private Helper helper;							// manages incoming requests
	final int iterations = 1;

	distributedProcess(int ID, int numberOfProcesses, asynchPort requestChannels[], asynchPort replyChannels[]) {
		super("distributedProcess"+ID);
		this.ID = ID;
		this.numberOfProcesses = numberOfProcesses;
		this.requestChannels = requestChannels;
		this.replyChannels = replyChannels;
		
		C = new Coordinator();
		helper = new Helper("helper"+ID);
		replyChannels[ID].setOwner(helper);
		requestChannels[ID].setOwner(this);
		helper.start();
	}

	class Helper extends TDThread {
	// manages requests from other distributed processes
		Helper(String name) {
			super(name);
		}
		public void run() { // handle requests from other distributedProcesses
			int messageCount = 0;

			while (messageCount < (numberOfProcesses-1)*iterations) {

				requestReplyMessage m = (requestReplyMessage) requestChannels[ID].receive();
				messageCount++;
				boolean defer = C.decideAboutDeferral(m);
				if (!defer) {
					//System.out.println("helper " + ID + " not deferring, send reply to "+m.ID);
					replyChannels[m.ID].send(new requestReplyMessage(0,ID,m.ID,false));
				}
			}
			return;
		}
	}

class Coordinator extends monitorSU {
// Synchronizes the distributed process and its helper
	private boolean requesting = false;			// true if requesting to enter CS
	private int highNumber;	
	private int number;						// highest number so far

	private boolean[] deferred = null;			// true means reply to other process has been deferred

	public Coordinator() {
		super("Coordinator" + ID);
		deferred = new boolean[numberOfProcesses];
		for (int i=0; i<numberOfProcesses; i++) deferred[i] = false;
	}
	
	public boolean decideAboutDeferral(requestReplyMessage m) {	
		enterMonitor("decideAboutDeferral");

		highNumber = Math.max(highNumber,m.number);
		boolean deferMessage = requesting && ((number < m.number)
			|| (number == m.number && ID < m.ID));
		if (deferMessage) {
			deferred[m.ID] = true;
		}
		exitMonitor();
		
		return deferMessage;
	}

	public int chooseNumberAndSetRequesting() {

		enterMonitor("chooseNumberAndSetRequesting");
		requesting = true;
		number = highNumber + 1;
		exitMonitor();
		return number;
	}

	public void resetRequestingAndReplyToDeferred() {
		enterMonitor("resetRequesting");
		requesting = false;
		for (int i=0; i < numberOfProcesses; i++)
			if (deferred[i]) {
				deferred[i] = false;		
				replyChannels[i].send(new requestReplyMessage(0,ID,i,false));
				//System.out.println(ID+" sent reply to "+i);
			}
		exitMonitor();
	}

}


	public void run() {

		int count = 0;
		TDThread t = TDThread.currentThread();
		while (count++<iterations) {
			//System.out.println(ID + " Before Critical Section");
			//System.out.flush();
			
			int number = C.chooseNumberAndSetRequesting();
			sendRequests(number);
			waitForReplies();
			ApplicationEvents.exerciseEvent("enter");
			//System.out.println(ID + " Leaving Critical Section");
			//System.out.flush();

			C.resetRequestingAndReplyToDeferred();

		}
		try {
			helper.join();
		}
		catch (InterruptedException e) {}
		return;
	}


	public void sendRequests(int number) {
		replyCount = 0;
		for (int i = 0; i < numberOfProcesses; i++) 
			if (i != ID) {
				requestChannels[i].send(new requestReplyMessage(number,ID,i,true));
			}
	}

	public void waitForReplies() {
		while (true) {
			//System.out.println(ID+" wait for reply");
			requestReplyMessage  m = (requestReplyMessage)replyChannels[ID].receive();
			//System.out.println(ID+" got reply");
			int receivedID = m.ID;
			replyCount++;
			if (replyCount == numberOfProcesses-1)
				break;
		}
	}
	
}

class requestReplyMessage {
	public int ID;
	public int destinationID;
	public int number;
	public boolean isRequest = false;
	public requestReplyMessage(int number, int ID, int destinationID, boolean isRequest) {
		this.ID = ID;this.destinationID = destinationID; this.number = number;this.isRequest = isRequest;
	}
}


