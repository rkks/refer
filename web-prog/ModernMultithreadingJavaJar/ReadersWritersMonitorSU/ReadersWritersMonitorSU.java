public final class ReadersWritersMonitorSU {
    public static void main (String args[]) {
		RWController c = new RWController(); 
		Reader r1 = new Reader(c,1);	Reader r2 = new Reader(c,2); 
		Reader r3 = new Reader(c,3);
		Writer w1 = new Writer(c,1);	Writer w2 = new Writer(c,2); 
		r1.start(); w1.start(); r2.start(); w2.start(); 
		r3.start();
		try {
		    r1.join (); r2.join (); 
		    r3.join ();
		    w1.join (); w2.join ();
		}
		catch (InterruptedException ex) {}
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

final class RWController extends monitorSU {
    /* Many readers or one writer with readers having a higher 
       priority.
    */
    private int readerCount = 0;
    private boolean writing = false;
    private conditionVariable readerQ = new conditionVariable("readerQ"); 
    private conditionVariable writerQ = new conditionVariable("writerQ");

    public RWController() {super("RWController");}

    public void read(int num) {
		startRead(); 
		//System.out.println("Reader" + num + " reading");
		try{Thread.sleep(10);}catch(InterruptedException e){}; 
		endRead(); 
    }
    public void write(int num) {
		startWrite(); 
		//System.out.println("Writer" + num + " writing");
		try{Thread.sleep(10);}catch(InterruptedException e){}; 
		endWrite();
    }

    private void startRead() {
		enterMonitor("startRead");
		//exerciseEvent("request");
		if (writing) {
		    readerQ.waitC();
		}
		readerCount++;
		exerciseEvent("startRead");
		readerQ.signalC_and_exitMonitor();
    }

    private void endRead() {
		enterMonitor("endRead");
		readerCount--;
		exerciseEvent("endRead");
		if (readerCount==0) {
		    writerQ.signalC_and_exitMonitor();
		    return;
		}
		exitMonitor();
    }

    private void startWrite() {
		enterMonitor("startWrite");
		//exerciseEvent("request");
		if (writing || readerCount>0) {
		    writerQ.waitC();
		}
		writing = true;
		exerciseEvent("startWrite");
		exitMonitor();
    }

    private void endWrite() {
		enterMonitor("endWrite");
		writing = false;
		exerciseEvent("endWrite");
		if (!(readerQ.empty())) {
		    readerQ.signalC_and_exitMonitor();
	   	 return;
		}
		else {
		    writerQ.signalC_and_exitMonitor();
	   	 return;
		}
    }
}


