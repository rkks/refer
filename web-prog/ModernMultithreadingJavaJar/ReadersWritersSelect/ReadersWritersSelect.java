public final class ReadersWritersSelect {
   public static void main (String args[]) {
		RWController c = new RWController(); 
		Reader r1 = new Reader(c,1);	Reader r2 = new Reader(c,2); 
		Reader r3 = new Reader(c,3);
		Writer w1 = new Writer(c,1);	Writer w2 = new Writer(c,2); 
		//Writer w3 = new Writer(c, 3);
		c.start();
		r1.start(); r2.start(); r3.start(); w1.start(); w2.start(); 

		//w3.start ();
		try {
		    r1.join (); r2.join (); 
	   	 r3.join ();
		    w1.join (); w2.join (); //w3.join ();
		    c.join();
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
		int x = c.read();
		//System.out.println("Reader"+num+" read " + x);
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

final class RWController extends TDThread {
//Strategy R>W.1 : Many readers or one writer, with readers having a higher priority
	private selectablePort startRead = new selectablePort("startread");
	private selectablePort endRead = new selectablePort("endread");
	private selectablePort startWrite = new selectablePort("startwrite");
	private selectablePort endWrite = new selectablePort("endwrite");
	private boolean writerPresent = false;
	private int readerCount = 0;
	private int sharedValue = 0;

	public RWController() {super("RWController");}

	public int read() {
		try {startRead.send();} catch(Exception e) {e.printStackTrace();}
		int value = sharedValue;
		try {endRead.send();} catch(Exception e) {e.printStackTrace();}
		return value;
	}

	public void write(int value) {
		try {startWrite.send();} catch(Exception e) {e.printStackTrace();}
		sharedValue = value;
		try {endWrite.send();} catch(Exception e) {e.printStackTrace();}
	}

 	public void run() {
 		//System.out.println("controller running");
  		try {
     		startRead.setOwner(this);
     		endRead.setOwner(this);
     		startWrite.setOwner(this);
     		endWrite.setOwner(this);
     		selectiveWait select = new selectiveWait();
     		select.add(startRead);				// alternative 1
     		select.add(endRead);					// alternative 2
     		select.add(startWrite);				// alternative 3
     		select.add(endWrite);				// alternative 4

     		int i = 0;
     		while(i<10) {
        		startRead.guard(!writerPresent);
       		endRead.guard(true);
        		startWrite.guard(!writerPresent && readerCount == 0 /*&& startRead.count() == 0*/);
        		endWrite.guard(true);
        		switch (select.choose()) {
     			case 1:	startRead.receive();
							++readerCount;
        					break;
  				case 2:	endRead.receive();
							--readerCount;
        					break;
   			case 3:	startWrite.receive();
							writerPresent = true;
        					break;
      		case 4:	endWrite.receive();
							writerPresent = false;
         				break;
         	}
         	i++;
   		}
   	} catch (Exception e) {}
   	//System.out.println("controller finished");
  	}
}
//Listing 5 11 Readers and Writers Using a selectiveWait.

