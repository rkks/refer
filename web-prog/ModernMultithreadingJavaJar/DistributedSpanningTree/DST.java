import java.util.*;

public class DST {
	static final int numberOfProcesses =3; // must be 2 or more
	public static void main(String[] args) {
		DistributedSpanningTree m = new DistributedSpanningTree(args);
		m.start();
		try {m.join();} catch ( java.lang.InterruptedException e ) { }
	}
}
