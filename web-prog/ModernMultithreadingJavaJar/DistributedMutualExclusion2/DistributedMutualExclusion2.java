interface Testable {
   void mainTest(String[] args);
}

public class DistributedMutualExclusion2 implements Testable {
	public static void main(String[] args) {
		DistributedMutualExclusion2 DME3 = new DistributedMutualExclusion2();
		DME3.mainTest(args);
	}
	public void mainTest(String[] args) {
		final int numberOfProcesses =3;
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

	private int number;								// sent on requests
	private int replyCount;							// replies received so far
	private int numberOfProcesses;
	private asynchPort[] requestChannels;		// requests sent to other processes
	private asynchPort[] replyChannels;			// replies received in response to requests
	private Coordinator C;							// monitor
	private Helper helper;							// manages incoming requests
	final int iterations = 1;

	asynchPort coordinate;
	asynchPort decidedAboutDeferral;
	asynchPort chosenNumber;
		
	distributedProcess(int ID, int numberOfProcesses, asynchPort requestChannels[], asynchPort replyChannels[]) {
		super("distributedProcess"+ID);
		this.ID = ID;
		this.numberOfProcesses = numberOfProcesses;
		this.requestChannels = requestChannels;
		this.replyChannels = replyChannels;

		coordinate = new asynchPort("coordinate"+ID);
		decidedAboutDeferral = new asynchPort("decidedAboutDeferral"+ID);
		chosenNumber = new asynchPort("chosenNumber"+ID);
		
		C = new Coordinator(coordinate, decidedAboutDeferral,chosenNumber/*,isDeferred*/);

		helper = new Helper("helper"+ID);
		replyChannels[ID].setOwner(helper);
		requestChannels[ID].setOwner(this);
		coordinate.setOwner(C);
		decidedAboutDeferral.setOwner(helper);
		chosenNumber.setOwner(this);
		C.start();
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
				coordinateMessage cm = new coordinateMessage(1,m);
				coordinate.send(cm);
				boolean defer = ((Boolean)decidedAboutDeferral.receive()).booleanValue();
				if (!defer) {
					//System.out.println("helper " + ID + " not deferring, send reply to "+m.ID);
					replyChannels[m.ID].send(new requestReplyMessage(0,ID,m.ID,false));
				}
				//else System.out.println("helper " + ID + " deferred reply to "+m.ID);
			}
			return;
		}
	}

	class Coordinator extends TDThread {
	// Synchronizes the distributed process and its helper
		private boolean requesting = false;			// true if requesting to enter CS
		private int highNumber;							// highest number so far
	   private boolean[] deferred = null;
		asynchPort coordinate;
		asynchPort decidedAboutDeferral;
		asynchPort chosenNumber;

		public Coordinator(asynchPort coordinate, asynchPort decidedAboutDeferral, 
	      asynchPort chosenNumber/*, asynchPort isDeferred*/) {
			super("Coordinator" + ID);
			this.coordinate = coordinate;
			this.decidedAboutDeferral = decidedAboutDeferral;
			this.chosenNumber = chosenNumber;
			
			deferred = new boolean[numberOfProcesses];
			for (int i=0; i<numberOfProcesses; i++) deferred[i] = false;
		}
	
		public void run() {
			int i = 0;
			int numDeferred=2;
			try {
				while(i<((numberOfProcesses-1)*iterations)+2) {
					coordinateMessage cm = (coordinateMessage) coordinate.receive();
					int choice = cm.choice;
					switch (choice) {
						case 1:	requestReplyMessage m = cm.msg;
									highNumber = Math.max(highNumber,m.number);
									boolean deferMessage = requesting && ((number < m.number)
										|| (number == m.number && ID < m.ID));
									if (deferMessage) {
										deferred[m.ID] = true;
									}
									decidedAboutDeferral.send(new Boolean(deferMessage));
									break;
						case 2:	requesting = true;
									number = highNumber + 1;
									chosenNumber.send(new Integer(number));
									break;
						case 3:  requesting = false;		
									for (int j=0; j < numberOfProcesses; j++) {
										if (j!=ID) {
											if (deferred[j]) {
												deferred[j] = false;
												replyChannels[j].send(new requestReplyMessage(0,ID,j,false));
												//System.out.println(ID+" sent deferred reply to "+j);
											}
											//else System.out.println(ID+" did not send deferred reply to "+j);
		 								}
									}
									break;
					}
					i++;
				}
			} catch (Exception e) {}
		}	
	}

	public void run() {
		int count = 0;
		while (count++<iterations) {
			//System.out.println(ID + " Before Critical Section");
			//System.out.flush();
			coordinateMessage cm = new coordinateMessage(2,null);
			coordinate.send(cm);
			int number = ((Integer)chosenNumber.receive()).intValue();
			sendRequests();
			waitForReplies();
			
			//System.out.println(ID + " Leaving Critical Section");
			//System.out.flush();
			cm = new coordinateMessage(3,null);
			coordinate.send(cm);
		}
		try {
			helper.join();C.join();
		}
		catch (InterruptedException e) {}
		return;
	}


	public void sendRequests() {
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

class coordinateMessage {
	public int choice;
	public requestReplyMessage msg;
	public coordinateMessage(int choice, requestReplyMessage msg) {
		this.choice = choice;this.msg = msg; 
	}	
}

