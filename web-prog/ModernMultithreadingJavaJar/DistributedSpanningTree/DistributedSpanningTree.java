public class DistributedSpanningTree extends TDThread {

    static final int numberOfProcesses = 3;
    boolean[][] neighbors = new boolean[numberOfProcesses][numberOfProcesses];
    childrenArray children = new childrenArray( numberOfProcesses );
    distributedNode[] DP = new distributedNode[numberOfProcesses];
    asynchPort[] messageChannels = new asynchPort[numberOfProcesses];
        
    DistributedSpanningTree( java.lang.String[] args )  {
        super( "DistributedSpanningTree" );

        for (int i = 0; i < numberOfProcesses; i++) {
            messageChannels[i] = new asynchPort( "messageChannel" + i );
	     }
	     
        /*
        //Here's a complete graph of 4 nodes. You must change the value of 
        //numberOfProcesses above to 4, and start a 4th process (DP[3]) below.
        neighbors[0][0] = false; neighbors[0][1] = true; neighbors[0][2] = true; neighbors[0][3] = true;
        neighbors[1][0] = true;  neighbors[1][1] = false;neighbors[1][2] = true; neighbors[1][3] = true;
        neighbors[2][0] = true;  neighbors[2][1] = true; neighbors[2][2] = false;neighbors[2][3] = true;
        neighbors[3][0] = true;  neighbors[3][1] = true; neighbors[3][2] = true; neighbors[3][3] = false;
        */

		  /*
        //Here's a complete graph of 4 nodes. You must change the value of 
        //numberOfProcesses above to 4, and start a 4th process (DP[3]) below.
        
        neighbors[0][0] = false;   neighbors[0][1] = true; neighbors[0][2] = true; neighbors[0][3] = true;
        neighbors[1][0] = true;    neighbors[1][1] = false;neighbors[1][2] = false;neighbors[1][3] = false;
        neighbors[2][0] = true;    neighbors[2][1] = false;neighbors[2][2] = false;neighbors[2][3] = true;
        neighbors[3][0] = true;    neighbors[3][1] = false;neighbors[3][2] = true; neighbors[3][3] = false;
        */
        
		  neighbors[0][0] = false; neighbors[0][1] = true; neighbors[0][2] = true; 
		  neighbors[1][0] = true; neighbors[1][1] = false; neighbors[1][2] = true; 
		  neighbors[2][0] = true; neighbors[2][1] = true; neighbors[2][2] = false;
	}
	
	public void run() {
        neighborMatrix m = new neighborMatrix( numberOfProcesses, neighbors );
        ApplicationEvents.exerciseEvent( m );
        (DP[0] = new distributedNode( 0, numberOfProcesses, messageChannels, true, children, neighbors )).start();
        (DP[1] = new distributedNode( 1, numberOfProcesses, messageChannels, false, children, neighbors )).start();
        (DP[2] = new distributedNode( 2, numberOfProcesses, messageChannels, false, children, neighbors )).start();
        //(DP[3] = new distributedNode( 3, numberOfProcesses, messageChannels, false, children, neighbors )).start();
        try {
            for (int i = 0; i < numberOfProcesses; i++) {
                DP[i].join();
            }
        } catch ( java.lang.InterruptedException e ) {
        }
        ApplicationEvents.exerciseEvent( children );
    }

}

class distributedNode extends TDThread
{

    private int ID;

    private int numberOfProcesses;

    private asynchPort[] messageChannels;

    final int iterations = 1;

    int parent = -1;

    boolean[][] neighbors;

    childrenArray children;

    boolean isRoot;

    distributedNode( int ID, int numberOfProcesses, asynchPort[] messageChannels, boolean isRoot, childrenArray children, boolean[][] neighbors )
    {
        super( "distributedNode" + ID );
        this.ID = ID;
        this.numberOfProcesses = numberOfProcesses;
        this.messageChannels = messageChannels;
        messageChannels[ID].setOwner( this );
        this.neighbors = neighbors;
        this.isRoot = isRoot;
        this.children = children;
    }

    public void run()
    {
        if (isRoot) {
            parent = ID;
            children.setChild( ID, ID );
            for (int i = 0; i < numberOfProcesses; i++) {
                if (neighbors[ID][i]) {
                    messageChannels[i].send( new Message( ID, i, "invite" ) );
                }
            }
        }
        int count = 0;
        int numNeighbors = 0;
        for (int i = 0; i < numberOfProcesses; i++) {
            if (neighbors[ID][i]) {
                numNeighbors++;
            }
        }
        while (true) {
            Message msg = (Message) messageChannels[ID].receive();
            if (msg.tag.equals( "invite" )) {
                if (parent == -1) {
                    count++;
                    parent = msg.src;
                    messageChannels[msg.src].send( new Message( ID, msg.src, "accept" ) );
                    for (int i = 0; i < numberOfProcesses; i++) {
                        ;
                        if (neighbors[ID][i] && i != msg.src) {
                            messageChannels[i].send( new Message( ID, i, "invite" ) );
                        }
                    }
                    if (count == numNeighbors) {
                        return;
                    }
                } else {
                    messageChannels[msg.src].send( new Message( ID, msg.src, "reject" ) );
                }
            } else {
                if (msg.tag.equals( "accept" ) || msg.tag.equals( "reject" )) {
                    if (msg.tag.equals( "accept" )) {
                        children.setChild( msg.src, ID );
                    }
                    count++;
                    if (count == numNeighbors) {
                        return;
                    }
                }
            }
        }
    }

}

class Message {
    public int src;
    public int destination;
    public java.lang.String tag;
    public Message( int src, int destination, java.lang.String tag ) {
        this.src = src;
        this.destination = destination;
        this.tag = tag;
    }
}
