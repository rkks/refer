public final class BoundedBufferMonitorSC {

	public static void main (String args[]) {
		BufferMonitor b = new BufferMonitor(2);
		Producer p1 = new Producer(b, 1);
		Producer p2 = new Producer(b, 2);
		Producer p3 = new Producer(b, 3);
		Consumer cl = new Consumer(b, 1);
		Consumer c2 = new Consumer(b, 2);
		Consumer c3 = new Consumer(b, 3);
		cl.start();p1.start(); 
		p2.start();c2.start();
		c3.start();p3.start();
		try {
		    cl.join(); p1.join();
		    p2.join(); c2.join(); 
		    c3.join(); p3.join();
		} 
 		catch (InterruptedException e) {}
	}
}

final class Producer extends TDThread {

	private BufferMonitor b;
	private int num;

	Producer(BufferMonitor b, int num) {
		super("Producer"+num);
 		this.b = b;
		this.num = num;
 	}

	public void run () {
			//System.out.println ("Producer # " + num + "  Running");

		for (int i = 0; i < 1; i++) {
			b.deposit(i);
			//System.out.println ("Producer # " + num + " deposited " + i);
		}
	}
}

final class Consumer extends TDThread {

	private BufferMonitor b;
	private int num;

	Consumer(BufferMonitor b, int num) {
		super("Consumer"+num);
		this.b = b;
		this.num = num;
	}

	public void run () {
 		int value = 0;
 		//System.out.println ("Consumer # " + num + " running");

		for (int i = 0; i < 1; i++) {
			value = b.withdraw();
			//System.out.println ("Consumer # " + num + " withdrew " + value);
		}
	}
}

final class BufferMonitor extends monitorSC {
	private int fullSlots = 0; 			// # of full slots in the buffer
	private int capacity = 0;
   private int[] buffer = null;
   private int in = 0, out = 0;

	private conditionVariable notFull;
	private conditionVariable notEmpty;

	public BufferMonitor(int capacity) {
		super("Buffer");
		this.capacity = capacity;
		buffer = new int[capacity];
		notFull = new conditionVariable("notFull");
		notEmpty = new conditionVariable("notEmpty");
	}

	public void deposit(int value) {
		enterMonitor("deposit");
		while (fullSlots == capacity)  {
			notFull.waitC();
		}
		buffer[in] = value;
      in = (in + 1) % capacity;
		++fullSlots;
		exerciseEvent("deposit");
		notEmpty.signalCall();
		exitMonitor();
}

	public int withdraw() {
		enterMonitor("withdraw");
		int value;
		while (fullSlots == 0) {
			notEmpty.waitC();
		}
		value = buffer[out];
      out = (out + 1) % capacity;
		--fullSlots;
		exerciseEvent("withdraw");
		notFull.signalCall();
		exitMonitor();
		return value;
	}
}
