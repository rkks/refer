public final class BoundedBufferSemaphores {

    public static void main (String args[]) {
		BufferSemaphore b1 = new BufferSemaphore(2);
		Producer p1 = new Producer (b1, 1);
		Producer p2 = new Producer (b1, 2);
		Producer p3 = new Producer (b1, 3);
		Consumer c1 = new Consumer (b1, 1);
		Consumer c2 = new Consumer (b1, 2);
		Consumer c3 = new Consumer (b1, 3);
		p1.start(); 
		p2.start();
		c1.start();
		c2.start(); 
		c3.start(); p3.start();
		try {
			c1.join(); p1.join();
			c2.join(); 
			p2.join();
			c3.join(); p3.join();	    
		} 
		catch (InterruptedException e) {}
    }
}

final class Producer extends TDThread {

    private BufferSemaphore b;
    private int num;
    private int ID;

    Producer (BufferSemaphore b, int num) {
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

    private BufferSemaphore b;
    private int num;

    Consumer (BufferSemaphore b, int num) {
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

final class BufferSemaphore {
    private int fullSlots = 0; 			// # of full slots in the buffer
    private int capacity = 0;
    private int[] buffer = null;
    private int in = 0, out = 0;

    private countingSemaphore notFull;
    private countingSemaphore notEmpty;
    private countingSemaphore mutexD;
    private countingSemaphore mutexW;

    public BufferSemaphore(int _capacity) {
		capacity = _capacity;
		buffer = new int[capacity];
		notFull = new countingSemaphore(capacity,"notFull");
		notEmpty = new countingSemaphore(0,"notEmpty");
		mutexD = new countingSemaphore(1,"mutexD");
		mutexW = new countingSemaphore(1,"mutexW");
    }

    public void deposit(int value) {
  		mutexD.P();
  		notFull.P();
		buffer[in] = value;
		in = (in + 1) % capacity;
		++fullSlots;
		//exerciseEvent("deposit");
		notEmpty.V();
		mutexD.V();
    }

    public int withdraw() {
		int value;
		mutexW.P();
		notEmpty.P();
		value = buffer[out];
		out = (out + 1) % capacity;
		--fullSlots;
		//exerciseEvent("withdraw");
		notFull.V();
		mutexW.V();

		return value;
    }
}
