public final class CyclicScheduler {
// Schedule k clients in succession, so first process is reactivated
// after kth client has been activated, but a client is never reactivated
// before it has finished
	public static final int size = 3;
	public static final int iterations = 2;
	// three clients; two rounds of activations
	public static void main (String args[]) {

		selectablePort[] next = new selectablePort[size];
		selectablePort[] finish = new selectablePort[size];		
		selectablePort[] start = new selectablePort[size];
		for (int i = 0; i<size; i++) {
			next[i] = new selectablePort("next"+i);
			finish[i] = new selectablePort("finish"+i);
			start[i] = new selectablePort("start"+i);
		}
		Client[] clients = new Client[size];
		Scheduler[] schedulers = new Scheduler[size];	
		for (int i = 0; i<size; i++) {
			clients[i] = new Client(finish,start,i);
			start[i].setOwner(clients[i]);
			schedulers[i] = new Scheduler(next,finish,start,i);
			next[i].setOwner(schedulers[i]);
			finish[i].setOwner(schedulers[i]);
			clients[i].start(); schedulers[i].start();
		}
		try{
			for (int i = 0; i<size; i++) {
				clients[i].join(); schedulers[i].join();
			}		
		}
 		catch (InterruptedException e) {System.exit(1);}
	}
}

final class Client extends TDThread {

	private selectablePort[] finish;
	private selectablePort[] start;
	private int ID;

	Client (selectablePort[] finish, selectablePort[] start, int ID) {
		super("Client"+ID);
  		this.finish = finish;
  		this.start = start;
 		this.ID = ID;
 	}

	public void run () {
		//System.out.println ("Client " + ID + "  Running");

		for (int i = 0; i < CyclicScheduler.iterations; i++) {
			try {
      		start[ID].receive();
      		finish[ID].send();
			}
			catch(Exception e) {}

		}
		//System.out.println ("Client " + ID + "  Done");
	}
}



final class Scheduler extends TDThread {
	private selectablePort[] next;
	private selectablePort[] finish;
	private selectablePort[] start;
	private int ID;
	private boolean done = false;
	private boolean ready = false;

	Scheduler (selectablePort[] next, selectablePort[] finish, selectablePort[] start, int ID) {
		super("Scheduler"+ID);
 		this.next = next;
  		this.finish = finish;
  		this.start = start;
 		this.ID = ID;
 	}


	public void run() {
 		//System.out.println ("scheduler " + ID + " running");
		try {
			selectiveWait select = new selectiveWait();
			select.add(next[ID]);
			select.add(finish[ID]);
			if (ID>0) {
       		next[ID].receive();
       	}
			for (int j=0; j<CyclicScheduler.iterations; j++) {
		      start[ID].send();
				if (!(ID==CyclicScheduler.size-1 && j==CyclicScheduler.iterations-1))
					// start another iteration 
					next[(ID+1) % CyclicScheduler.size].send();
		      done = false;
				ready = false;
				while (!(done && ready) && (!(done && j==CyclicScheduler.iterations-1))) {
				// Scheduler won't get another call to next if all iterations are complete.
				// if iterations are complete, stop after client is finished.
					finish[ID].guard(!done);
					next[ID].guard(!ready);
					int choice = select.choose();
					switch (choice) {
						case 1:	next[ID].receive();
									ready = true;
									break;
						case 2: 	finish[ID].receive();
									done = true;
									break;	
					}
				}
			}
		} catch (InterruptedException e) {}
		//System.out.println("scheduler " + ID + " done");
	}
}
