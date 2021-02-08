public final class SharedVariables {

	public static void main (String args[]) {
		sharedInteger s = new sharedInteger(0,"s");
		sharedBoolean b = new sharedBoolean(true,"b");
		sharedDouble d = new sharedDouble(1.0,"d");
		sharedLong l = new sharedLong(1,"l");
		binarySemaphore mutex = new binarySemaphore(1,"mutex");
		Reader r1 = new Reader (s,b,d,l,1,mutex);
		Reader r2 = new Reader (s,b,d,l,2,mutex);
		Writer w1 = new Writer (s,b,d,l,1,mutex);;
		r1.start();
		w1.start(); 
		r2.start();
		try {
		    r1.join(); r2.join(); w1.join();
		}
 		catch (InterruptedException e) {}
	}
}

final class Reader extends TDThread {

	sharedInteger s;
	sharedBoolean b;
	sharedDouble d;
	sharedLong l;
	private int ID;
	private binarySemaphore mutex;

	Reader (sharedInteger s, sharedBoolean b, sharedDouble d, sharedLong l, int ID, binarySemaphore mutex) {
 		this.s = s;	this.b = b; this.d = d; this.l = l; this.ID = ID;this.mutex = mutex;
 	}

	public void run () {
		//System.out.println ("Reader # " + ID + "  Running");
		for (int i = 0; i < 1; i++) {
			mutex.P();
			System.out.println("Reader # " + ID + " read "+s.Read()+" for "+s.getName());
			b.Write(true);
			d.Write(2.0);
			l.Write(2);
			mutex.V();
		}
	}
}

final class Writer extends TDThread {

	sharedInteger s;
	sharedBoolean b;
	sharedDouble d;
	sharedLong l;
	private int ID;
	private binarySemaphore mutex;

	Writer (sharedInteger s, sharedBoolean b, sharedDouble d, sharedLong l, int ID, binarySemaphore mutex) {
 		this.s = s; this.b = b; this.d = d; this.l = l; this.ID = ID; this.mutex = mutex;
 	}

	public void run () {
		//System.out.println ("Writer # " + ID + "  Running");
		for (int i = 0; i < 1; i++) {
			mutex.P();
			s.Write(ID);
			System.out.println("Writer # " + ID + " wrote " + ID + " to "+s.getName());
			b.Write(false);
			d.Write(3.0);
			l.Write(3);
			mutex.V();
		}
	}
}

