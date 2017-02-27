package model;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;
import control.EstimatorInterface;

public class DummyLocalizer implements EstimatorInterface {
	private final int LEFT = 0, DOWN = 1, RIGHT = 2, UP = 3;
	
	private int rows, cols, head, trueX, trueY, states, heading;
	private int[] sensorXY;
	
	private double[] forward;
	private double[][] matrixT;
	private double[][][] matrixO;
	private ArrayList<State> stateMapping;
	private double[][] tTranspose;
	
	public DummyLocalizer( int rows, int cols, int head) {
		this.rows = rows;
		this.cols = cols;
		this.head = head;
		this.trueX = 1;
		this.trueY = 1;
		this.heading = (new Random()).nextInt(4);
		this.states = head * cols * rows;
				
		forward = new double[states];
		matrixO = new double[(cols * rows) + 1][states][states];
		matrixT = new double[states][states];
		stateMapping = new ArrayList<State>();

		buildStateVector();
		initiateMatrixT();
		initiateMatrixO();
		initateForward();
		
		tTranspose = transposeMatrix(matrixT);
	}
	
	private void initateForward() {
		int index = stateMapping.indexOf(new State(trueX, trueY, LEFT));
		
		forward[index++] = 0.25;
		forward[index++] = 0.25;
		forward[index++] = 0.25;
		forward[index++] = 0.25;
	}
	
	private void initiateMatrixO() {
		//Case: 'nothing'
		for(int state = 0; state < states; state++){
			State currState = stateMapping.get(state);
			Position posState = new Position(currState.getX(), currState.getY());
			
			ArrayList<Position> n_Ls = getNeighbours(posState);
			ArrayList<Position> n_Ls2 = n_Ls2(posState, n_Ls);
			
			matrixO[16][state][state] = 1 - 0.1 - (n_Ls.size() * 0.05) - (n_Ls2.size() * 0.025);
		}
		
		normalizeMatrix(matrixO[16]);

		//P(e_t | x_t)
		for(int evidence = 0; evidence < rows * cols; evidence++) {
			Position evidencePos = new Position(evidence/4, evidence%4);
			System.out.println("Evidence: " + evidence + ", pos: (" + evidencePos.x + ", " + evidencePos.y + ")");
			for(int state = 0; state < states; state++){
				if(ifSameState(evidence, state)){
					matrixO[evidence][state][state] = 0.1;
					continue;
				}
				
				State currState = stateMapping.get(state);
				Position posState = new Position(currState.getX(), currState.getY());

				ArrayList<Position> n_Ls = getNeighbours(posState);
				ArrayList<Position> n_Ls2 = n_Ls2(posState, n_Ls);
								
				if(n_Ls.contains(evidencePos)) {
					matrixO[evidence][state][state] = 0.05;
					
					continue;
				}
				
				if(n_Ls2.contains(evidencePos)) {
					matrixO[evidence][state][state] = 0.025;
					
					continue;
				}
			}
			
			normalizeMatrix(matrixO[evidence]);
		}
	}
	
	private boolean ifSameState(int evidence, int state){
		return evidence == state/4;
	}
	
	private void normalizeMatrix(double[][] matrix){
		double sum = 0;
		
		for(int i = 0; i < states; i++){
			sum += matrix[i][i];
		}
		
		for(int i = 0; i < states; i++){
			matrix[i][i] /= sum;
		}
	}
	
	private void buildStateVector() {
		for(int i = 0; i < cols; i++) {
			for(int j = 0; j < rows; j++) { 
				for(int h = 0; h < head; h++) {
					stateMapping.add(new State(i, j, h));
				}
			}
		}
	}
	
	private boolean getCross(Position origin, Position to) {
		int fromX = origin.x;
		int fromY = origin.y;
		
		int toX = to.x;
		int toY = to.y;
		
		if(fromX == toX && fromY - 1 == toY) {
			return true;
		}
		
		else if(fromX == toX && fromY + 1 == toY) {
			return true;
		}
		
		else if(fromY == toY && fromX - 1 == toX) {
			return true;
		}
		
		else if(fromY == toY && fromX + 1 == toX) {
			return true;
		}
		
		return false;
	}
	
	private boolean inCorner(Position pos) {
		int x = pos.x;
		int y = pos.y;
		
		return (x == 0 || x == cols - 1) && (y == 0 || y == rows - 1);
	}
	
	private boolean validStateTransition(State from, State to) {
		int fromX = from.getX();
		int fromY = from.getY();
		
		int toX = to.getX();
		int toY = to.getY();
		
		int toDir = to.getHeading();
		
		switch(toDir){
		case UP:
			return fromY - toY == 1;
			
		case LEFT:
			return fromX - toX == 1;
			
		case RIGHT:
			return toX - fromX == 1;
			
		case DOWN:
			return toY - fromY == 1;
		}
		
		return false;
	}
	
	private boolean nextToWall(Position pos) {
		int x = pos.x;
		int y = pos.y;
		
		return x == 0 || x == cols - 1 || y == 0 || y == rows - 1;
	}
	
	private void initiateMatrixT(){
		for(int i = 0; i < states; i++){
			for(int j = 0; j < states; j++){
				if(i == j){
					matrixT[i][j] = 0;
					continue;
				}
				
				State from = stateMapping.get(i);
				State to = stateMapping.get(j);
				
				Position posFrom = new Position(from.getX(), from.getY());
				Position posTo = new Position(to.getX(), to.getY());
				
				if(!getCross(posFrom, posTo)) {
					matrixT[i][j] = 0;
					
					continue;
				}
				
				if(!validStateTransition(from, to)) {
					matrixT[i][j] = 0;
					
					continue;
				}
				
				if(inCorner(posFrom)) {
					if(!encounterWall(posFrom, from.getHeading())) {
						if(from.getHeading() == to.getHeading()) {
							matrixT[i][j] = 0.7;
						}
						
						else {
							matrixT[i][j] = 0.3;
						}
					}
					
					else {
						matrixT[i][j] = 0.5;
					}
					
					continue;
				}
						
				if(encounterWall(posFrom, from.getHeading())) {
					matrixT[i][j] = 1/(double)3;
					
					continue;
				}
				
				if(nextToWall(posFrom)) {
					if(from.getHeading() == to.getHeading()) {
						matrixT[i][j] = 0.7;
					}
					
					else {
						matrixT[i][j] = 0.15;
					}
				}
				
				else {
					if(from.getHeading() == to.getHeading()) {
						matrixT[i][j] = 0.7;
					}
					
					else {
						matrixT[i][j] = 0.1;
					}
				}
			}
		}
	}
	
	public int getNumRows() {
		return rows;
	}
	
	public int getNumCols() {
		return cols;
	}
	
	public int getNumHead() {
		return head;
	}
	
	public double getTProb( int x, int y, int h, int nX, int nY, int nH) {
		State from = new State(x, y, h);
		State to = new State(nX, nY, nH);
		
		int i = stateMapping.indexOf(from);
		int j = stateMapping.indexOf(to);
		
		return matrixT[i][j];
	}

	public double getOrXY( int rX, int rY, int x, int y) {
		int index = rX * 4 + rY;
		
		State nothing = new State(x, y, UP);
		int stateIndex = stateMapping.indexOf(nothing);
		
		if(rX == -1 || rY == -1) {
			System.out.println("In the void of anus.");
			
			return matrixO[16][stateIndex][stateIndex] * 4;
		}
				
		return matrixO[index][stateIndex][stateIndex] * 4;
	}

	public int[] getCurrentTruePosition() {
		return new int[]{trueX, trueY};
	}
	
	private void move() {
		setNewHeading();
		
		System.out.println("Heading: " + heading);

		switch(heading) {
		case UP: trueY--;
			break;
			
		case DOWN: trueY++;
			break;
			
		case LEFT: trueX--;
			break;
			
		case RIGHT: trueX++;
			break;
		}
		
		System.out.println("X: " + trueX + " Y: " + trueY);
	}
	
	private boolean illegalPosition(Position pos) {
		int x = pos.x;
		int y = pos.y;
		
		return x < 0 || x > cols - 1 || y < 0 || y > rows - 1;
	}
	
	private ArrayList<Position> getNeighbours(Position pos) {
		ArrayList<Position> neighbours = new ArrayList<Position>();
		int x = pos.x;
		int y = pos.y;
		
		for(int i = -1; i <= 1; i++){
			for(int j = -1; j <= 1; j++) {
				if(i == 0 && j == 0){
					continue;
				}

				Position neighbour = new Position(x-i, y-j);
				if(!illegalPosition(neighbour)){
					neighbours.add(neighbour);
				}
			}
		}
		return neighbours;
	}
	
	private boolean encounterWall(Position pos, int direction) {
		int x = pos.x;
		int y = pos.y;
		
		switch(direction) {
		case UP:
			if(y == 0) {
				return true;
			}
			break;
			
		case DOWN:
			if(y == rows - 1) {
				return true;
			}
			break;

		case LEFT:
			if(x == 0) {
				return true;
			}
			break;

		case RIGHT:
			if(x == cols - 1) {
				return true;
			}
			break;

		}
		
		return false;
	}
	
	private int freeDirection(Position pos, int direction) {
		direction = (new Random()).nextInt(head);
		
		while(true) {
			if(encounterWall(pos, direction)) {
				direction = (new Random()).nextInt(head);
			}
			
			else {
				break;
			}
		}
		
		return direction;
	}

	private void setNewHeading(){
		Position pos = new Position(trueX, trueY);
		
		if(encounterWall(pos, heading)) {
			heading = freeDirection(pos, heading);
		}
		
		else {
			double probability = (new Random()).nextDouble();
			
			if(probability <= 0.3) {
				heading = freeDirection(pos, heading);
			}
		}
	}
	
	private ArrayList<Position> n_Ls() {
		return getNeighbours(new Position(trueX, trueY));
	}
	
	private boolean inList(Position origin, Position pos, ArrayList<Position> list){
		if(pos.equals(origin)) {
			return true;
		}
		
		for(Position element : list) {
			if(element.equals(pos)) {
				return true;
			}
		}
		
		return false;
	}
	
	private ArrayList<Position> n_Ls2(Position origin, ArrayList<Position> neighbours) {
		ArrayList<Position> neighbours_L2 = new ArrayList<Position>();

		for(Position pos: neighbours) {
			List<Position> current = getNeighbours(pos);
			
			for(Position element : current) {
				if(!inList(origin, element, neighbours) && !inList(origin, element, neighbours_L2) && !origin.equals(element)) {
					neighbours_L2.add(element);
				}
			}
		}
		
		return neighbours_L2;		
	}

	public int[] getCurrentReading() {
		/*
		sensorXY = new int[]{trueX, trueY};
		
		return sensorXY;
		*/
		
		Random rand = new Random();
		double probability = rand.nextDouble();
		
		ArrayList<Position> neighbours = n_Ls();
		List<Position> neighbours_L2 = n_Ls2(new Position(trueX, trueY), neighbours);

		double n_Ls1 = neighbours.size() * 0.05;
		double n_Ls2 = neighbours_L2.size() * 0.025;
		
		if(probability < 0.1) {
			sensorXY = new int[]{trueX, trueY};
		}
		
		else if(probability >= 0.1 && probability < 0.1 + n_Ls1) {
			int pos = rand.nextInt(neighbours.size());
			
			sensorXY = new int[]{neighbours.get(pos).x, neighbours.get(pos).y};
		}
		
		else if(probability >= 0.1 + n_Ls1 && probability < 0.1 + n_Ls1 + n_Ls2) {
			int pos = rand.nextInt(neighbours_L2.size());
			
			sensorXY = new int[]{neighbours_L2.get(pos).x, neighbours_L2.get(pos).y};
		}
		
		else {
			return null;
		}
		
		return sensorXY;
		
	}

	public double getCurrentProb(int x, int y) {
		int index = stateMapping.indexOf(new State(x, y, LEFT));
		
		double sum = forward[index++];
		sum += forward[index++];
		sum += forward[index++];
		sum += forward[index++];
		
		return sum;
	}
	
	private void forwardAlgorithm() {
		int evidence;
		if(sensorXY == null) {
			evidence = 16;
		}
		else{
			int x = sensorXY[0];
			int y = sensorXY[1];
			
			evidence = stateMapping.indexOf(new State(x,y,LEFT))/4;
		}
		double[][] O_t1 = matrixO[evidence];
		double[][] resultOT = multiplyMatrix(O_t1, tTranspose);
		forward = multiplyMatrixVector(resultOT, forward);
		normalizeForward();
	}
	
	private void normalizeForward(){
		double sum = 0;
		
		for(int i = 0; i < states; i++){
			sum += forward[i];
		}
		
		for(int i = 0; i < states; i++){
			forward[i] /= sum;
		}
	}
	
	private double[][] transposeMatrix(double [][] m){
        double[][] temp = new double[m[0].length][m.length];
        for (int i = 0; i < m.length; i++)
            for (int j = 0; j < m[0].length; j++)
                temp[j][i] = m[i][j];
        return temp;
    }
    
	private double[] multiplyMatrixVector(double[][] a, double[] x) {
        int m = a.length;
        int n = a[0].length;
        if (x.length != n) throw new RuntimeException("Illegal matrix dimensions.");
        double[] y = new double[m];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                y[i] += a[i][j] * x[j];
        return y;
    }
    
	private double[][] multiplyMatrix(double[][] a, double[][] b) {
        int m1 = a.length;
        int n1 = a[0].length;
        int m2 = b.length;
        int n2 = b[0].length;
        if (n1 != m2) throw new RuntimeException("Illegal matrix dimensions.");
        double[][] c = new double[m1][n2];
        for (int i = 0; i < m1; i++)
            for (int j = 0; j < n2; j++)
                for (int k = 0; k < n1; k++)
                    c[i][j] += a[i][k] * b[k][j];
        return c;
    }
	
	public void update() {
		move();
		
		forwardAlgorithm();
	}
	
	public class Position implements Comparable<Position> {
		private int x, y;
		
		public Position(int x, int y) {
			this.x = x;
			this.y = y;
		}
		
		@Override
		public boolean equals(Object obj) {
			return x == ((Position)obj).x && y == ((Position)obj).y;
		}
		
		public String toString() {
			return x + ", " + y;
		}

		@Override
		public int compareTo(Position position) {
			if((position.x - x == 0) && (position.y - y == 0)) {
				return 0;
			} 
			
			else if(position.x < x) {
				return -1;
			}
			
			return 1;
		}
	}
}