public final class DiningPhilosophers {
// dining philosophers with a deadlock
    public static void main (String args[]) {
		countingSemaphore stick0 = new countingSemaphore(1,"stick0");
		countingSemaphore stick1 = new countingSemaphore(1,"stick1");
		countingSemaphore stick2 = new countingSemaphore(1,"stick2");
		countingSemaphore stick3 = new countingSemaphore(1,"stick3");
		
		Philosopher p0 = new Philosopher (stick0, stick1, 0);
		Philosopher p1 = new Philosopher (stick1, stick2, 1);
		Philosopher p2 = new Philosopher (stick2, stick3, 2);
		Philosopher p3 = new Philosopher (stick3, stick0, 3);
		// replace the preceding line with the following line to remove the deadlock
		//Philosopher p3 = new Philosopher (stick0, stick3, 3);

		p3.start();	
		p2.start();	
		p0.start(); p1.start(); 
		try {
		    p0.join(); p1.join(); p2.join(); 
		    p3.join(); 
		}
		catch (InterruptedException e) {}
    }
}

final class Philosopher extends TDThread {

    private countingSemaphore lstick, rstick;
    private int num;

    Philosopher (countingSemaphore lstick, countingSemaphore rstick, int num) {
    	super("Philosopher"+num);
		this.lstick = lstick;
		this.rstick = rstick;
		this.num = num;
    }

    public void run () {
		//System.out.println ("Philosopher # " + num + "  Running");
		lstick.P();
		rstick.P();
		//System.out.println ("Philosopher # " + num + " Eating ");
		lstick.V();
		rstick.V();
    }
}

