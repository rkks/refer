public final class BoundedBufferSelect {

	public static void main (String args[]) {

		selectablePort deposit = new selectablePort("deposit");
		selectablePort withdraw = new selectablePort("withdraw");
		selectablePort replyPortC1 = new selectablePort("replyPortC1");
		selectablePort replyPortC2 = new selectablePort("replyPortC2");
		selectablePort replyPortC3 = new selectablePort("replyPortC3");
		BufferSelect b = new BufferSelect(deposit,withdraw,replyPortC1,replyPortC2,replyPortC3, 2);
		Producer p1 = new Producer (deposit, 1);
		Producer p2 = new Producer (deposit, 2);
		Producer p3 = new Producer (deposit, 3);
		Consumer c1 = new Consumer (withdraw, replyPortC1,1);
		Consumer c2 = new Consumer (withdraw, replyPortC2,2);
		Consumer c3 = new Consumer (withdraw, replyPortC3,3);
		deposit.setOwner(b);
		withdraw.setOwner(b);
		replyPortC1.setOwner(c1);
		replyPortC2.setOwner(c2);
		replyPortC3.setOwner(c3);

		b.start();
		c1.start();
		c2.start();
		c3.start();
		p1.start();
		p2.start();
		p3.start();
		try{c1.join(); p1.join(); c2.join(); p2.join(); c3.join(); p3.join(); b.join();}
 		catch (InterruptedException e) {System.exit(1);}
	}
}

final class Producer extends TDThread {

	private selectablePort deposit;
	private int num;

	Producer (selectablePort deposit, int num) {
		super("Producer"+num);
 		this.deposit = deposit;
 		this.num = num;
 	}

	public void run () {
		//System.out.println ("Producer  Running");

		for (int i = 0; i < 1; i++) {
			try {
				//System.out.println ("Producer calling deposit.");
				deposit.send(new Integer(num));
			}
			catch(Exception e) {}
			//System.out.println ("Producer # " + num + " deposited " + num);
		}
		//System.out.println ("Producer  Done");
	}
}

final class Consumer extends TDThread {

	private selectablePort withdraw;
	private selectablePort replyPort;
	private int num;

	Consumer (selectablePort withdraw, selectablePort replyPort, int num) {
		super("Consumer"+num);
		this.withdraw = withdraw;
		this.replyPort = replyPort;
		this.num = num;
	}

	public void run () {
		Object value = null;
 		//System.out.println ("Consumer running");

		for (int i = 0; i < 1; i++) {
         try {
 				//System.out.println ("Consumer " + num +  " calling withdraw.");
				withdraw.send(new Integer(num));
				//System.out.println("Consumer " + num +  " waiting to receive");
				value = replyPort.receive();
			}
			catch (Exception e) {}
			//System.out.println ("Consumer # " + num + " withdrew " + ((Integer)value).intValue()); //  + value);
		}
		//System.out.println ("Consumer  Done");
	}
}

final class BufferSelect extends TDThread {
	private selectablePort deposit, withdraw, replyPortC1, replyPortC2, replyPortC3;
	private int fullSlots=0;
	private int capacity = 0;
	private Object[] buffer = null;
	private int in = 0, out = 0;

	public BufferSelect(selectablePort deposit, selectablePort withdraw, 
	   selectablePort replyPortC1, selectablePort replyPortC2, 
	   selectablePort replyPortC3, int capacity) {
  		super("bufferSelect");
		this.deposit = deposit; this.withdraw = withdraw; 
		this.replyPortC1 = replyPortC1; this.replyPortC2 = replyPortC2;
		this.replyPortC3 = replyPortC3;
    		this.capacity = capacity;
			buffer = new Object[capacity];
  	}
	public BufferSelect(selectablePort deposit, selectablePort withdraw, 
	   selectablePort replyPortC1, selectablePort replyPortC2, int capacity) {
		this.deposit = deposit; this.withdraw = withdraw; 
		this.replyPortC1 = replyPortC1; this.replyPortC2 = replyPortC2;;
    		this.capacity = capacity;
			buffer = new Object[capacity];
  	}


	public void run() {
 		//System.out.println ("Buffer running");
 		int i = 0;
		try {
			selectiveWait select = new selectiveWait();
			select.add(deposit);			// 1
			select.add(withdraw);		// 2
			while(i<6) {
				withdraw.guard(fullSlots>0);
				deposit.guard(fullSlots<capacity);
				int choice = select.choose();
				switch (choice) {
					case 1:	Object o = deposit.receive();
								buffer[in] = o;
								in = (in + 1) % capacity;
								++fullSlots;
								break;
					case 2:	int ID = ((Integer)withdraw.receive()).intValue();
								//System.out.println("Buffer receive ID " + ID);
								Object value = buffer[out];
								if (ID==1) {
									//System.out.println("Buffer reply to Consumer"+ID);
									replyPortC1.send(value);
								}
								else if (ID==2) {
									//System.out.println("Buffer reply to Consumer"+ID);
									replyPortC2.send(value);
								}
								else {
									//System.out.println("Buffer reply to Consumer"+ID);
									replyPortC3.send(value);
								}
								out = (out + 1) % capacity;
								--fullSlots;
								break;
				}
				i++;
			}
		} catch (InterruptedException e) {}
		//System.out.println("Buffer done");
	}
}
