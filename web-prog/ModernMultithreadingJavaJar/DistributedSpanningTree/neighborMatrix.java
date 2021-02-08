public class neighborMatrix {
	private boolean[][] neighbors;
	private int size;
	public neighborMatrix(int size, boolean[][] neighbors) {
		this.size = size;
		this.neighbors = neighbors;
	}
	public boolean[][] getNeighbors() {return neighbors;}
	public String toString() {
		StringBuffer temp = new StringBuffer();
		temp.append("[");
		for (int i=0; i<size-1; i++) {
			for (int j=0; j<size-1; j++)
				temp.append((neighbors[i][j])+" ");
			temp.append(neighbors[i][size-1]);
			temp.append(",");		
		}
		for (int j=0; j<size-1; j++)
			temp.append((neighbors[size-1][j])+" ");
		temp.append(neighbors[size-1][size-1]);
		temp.append("]");
		return temp.toString();
	}
}
