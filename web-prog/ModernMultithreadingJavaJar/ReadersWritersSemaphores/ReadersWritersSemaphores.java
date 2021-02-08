public final class ReadersWritersSemaphores {
   public static void main (String args[]) {
   	RWController c = new RWController(); 
		Reader r1 = new Reader(c,1);	Reader r2 = new Reader(c,2); Reader r3 = new Reader(c,3);
		Writer w1 = new Writer(c,1);	Writer w2 = new Writer(c,2); 
		r1.start(); w1.start(); r2.start(); w2.start(); r3.start();
		try {
	    r1.join(); r2.join(); r3.join();
	    w1.join(); w2.join();
		} 
		catch (InterruptedException e) {}
	}
}

final class Reader extends TDThread {
	private RWController c;
	private int num;
	Reader (RWController c, int num) {
		super("Reader"+num);
		this.c = c; this.num = num; 
	}
	public void run () {
		//System.out.println("Reader"+num+" running");
		c.read(num);
		//System.out.println("Reader"+num+" finished");
	}
}

final class Writer extends TDThread {
	private RWController c;
	private int num;
	Writer (RWController c, int num) {
		super("Writer"+num);
		this.c = c; this.num = num; 
	}
	public void run () {		
		//System.out.println("Writer"+num+" running");
		c.write(num);
		//System.out.println("Writer"+num+" finished");
	}
}

final class RWController{
/* Many readers or one writer with readers having a higher priority */
	int activeReaders = 0; 						// number of active readers
	int activeWriters = 0;						// number of active writers
	int waitingWriters = 0; 					// number of waiting writers
	int waitingReaders = 0; 					// number of waiting readers
	countingSemaphore mutex; 					// mutual exclusion
	countingSemaphore readers_que;			// queue for waiting readers
	countingSemaphore writers_que;			// queue for waiting writers
	int x=0;       								// x is shared data

	public RWController() {
		mutex = new countingSemaphore(1,"mutex");
		readers_que = new countingSemaphore(0,"readers_que");
		writers_que = new countingSemaphore(0,"writers_que");
	}

	public void read(int num) {
		mutex.P();
		if (activeWriters > 0) {
			waitingReaders++;
			mutex.V();
			readers_que.P();
		}
		activeReaders++;
		if (waitingReaders > 0) {
			waitingReaders--;
			readers_que.V();			// pass the baton
		}							
		else {
			mutex.V();
		}

		//System.out.println("Reader" + num + " reading");
		//try{Thread.sleep(10);}catch(InterruptedException e){}; 

		mutex.P();
		activeReaders--;
		if (activeReaders == 0 && waitingWriters > 0) {
			  waitingWriters--;
		  	  writers_que.V();		// pass the baton
		}
		else
			mutex.V();
			
	}				

	public void write(int num) {
		mutex.P();
		if (activeReaders > 0 || activeWriters > 0 ) {
			waitingWriters++;
			mutex.V();
			writers_que.P();
		}
		activeWriters++;
		mutex.V();
		
		//System.out.println("Writer" + num + " writing");
		//try{Thread.sleep(10);}catch(InterruptedException e){}; 
		
		mutex.P();
		activeWriters--;
		if (waitingReaders > 0) {
			waitingReaders--;
			readers_que.V();
		}
		else if (waitingWriters > 0) {
			waitingWriters--;
		   writers_que.V();
		}
		else
			mutex.V();
	}
}


