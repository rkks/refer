public class childrenArray {
	private int[] children;
	private int size;
	public childrenArray(int size) {
		this.size = size;
		children = new int[size];
		for (int i=0; i<size; i++)
			children[i] = -1;
	}
	public int[] getChildren() {return children;}
	public void setChild(int i, int j) {children[i] = j;}
	public String toString() {
		StringBuffer temp = new StringBuffer();
		temp.append("[");
		for (int i=0; i<size-1; i++)
			temp.append(children[i]);
		temp.append(children[size-1]);
		temp.append("]");
		return temp.toString();
	}
}
