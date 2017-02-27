package model;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import control.EstimatorInterface;

public class DummyLocalizer implements EstimatorInterface {
	private State currentState;
	private int rows, cols, head, trueX, trueY, states;
	private int heading;
	static final int LEFT = 0, DOWN = 1, RIGHT = 2, UP = 3;
	private double[][] matrixO, matrixT;
	private ArrayList<State> stateMapping;
	
	public DummyLocalizer( int rows, int cols, int head) {
		this.rows = rows;
		this.cols = cols;
		this.head = head;
		this.trueX = 1;
		this.trueY = 1;
		this.heading = (new Random()).nextInt(4);
		this.states = head*cols*rows;
		
		matrixO = new double[states][states];
		matrixT = new double[states][states];
		
		stateMapping = new ArrayList<State>();
		buildStateVector();
		initiateMatrixT();
		
	}
	
	
	
	private void buildStateVector() {
		for(int i = 0; i < cols; i++){ 			//X
			for(int j = 0; j < rows; j++){ 		//Y
				for(int h = 0; h < head; h++){ 	//Heading
					stateMapping.add(new State(i,j,h));
				}
			}
		}
	}
	
	private boolean getCross(){
		return false;
		
	}
	
	private void initiateMatrixT(){
		for(int i = 0; i < states; i++){
			for(int j = 0; j < states; j++){
				//State 0 = 0,0,0. State 1 = 0,0,1, State 2 = 0,0,2, State 3 = 0,0,3, State 4 = 1,0,0 (x,y,heading)
				//Position pos = new Position(i,j);
				
				if(i == j){
					matrixT[i][j] = 0;
					continue;
				}
				
				State from = stateMapping.get(i);
				State to = stateMapping.get(j);
				
				Position posFrom = new Position(from.getX(), from.getY());
				Position posTo = new Position(to.getX(), to.getY());
				
				List<Position> fromNeighbours = getNeighbours(posFrom);
				
				if(!fromNeighbours.contains(posTo)){
					matrixT[i][j] = 0;
					continue;
				}
						
				if(encounterWall(posFrom, from.getHeading())){
					
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
		//System.out.println(x + " , " + y + " , " + h+ " , " + nX+ " , " + nY + " , " + nH);
		return 0.1;
		//return 0;
	}

	public double getOrXY( int rX, int rY, int x, int y) {
		return 0.1337;
	}

	public int[] getCurrentTruePosition() {
		int[] ret = new int[2];
		ret[0] = trueY;
		ret[1] = trueX;
		return ret;

	}
	
	private void move(int direction) {
		setNewHeading();
		switch(direction) {
		case UP: trueY--;
			break;
			
		case DOWN: trueY++;
			break;
			
		case LEFT: trueX--;
			break;
			
		case RIGHT: trueX++;
			break;
		}
		
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
				System.out.println("Direction: " + direction);
			}else {
				break;
			}
		}
		return direction;
	}
	//O: State*State matrix
	//Diagonal = states probability
	//Rest = 0
	
	//T: State*State matrix
	//
	
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
	
	
	private boolean inList(Position pos, ArrayList<Position> list){
		if(pos.equals(new Position(trueX, trueY))){
			return true;
		}
		for(Position element : list){
			if(element.equals(pos)){
				return true;
			}
		}
		return false;
	}
	
	private ArrayList<Position> n_Ls2(ArrayList<Position> neighbours) {
		ArrayList<Position> neighbours_L2 = new ArrayList<Position>();

		for(Position pos: neighbours){
			List<Position> current = getNeighbours(pos);
			
			for(Position element : current){
				if(!inList(element, neighbours) && !inList(element, neighbours_L2)){
					neighbours_L2.add(element);
				}

				
			}
		}
		
		return neighbours_L2;		
	}

	public int[] getCurrentReading() {
		Random rand = new Random();
		double probability = rand.nextDouble();
		ArrayList<Position> neighbours = n_Ls();
		List<Position> neighbours_L2 = n_Ls2(neighbours);

		double n_Ls1 = neighbours.size() * 0.05;
		double n_Ls2 = neighbours_L2.size() * 0.025;
		
		if(probability < 0.1){
			return new int[]{trueX, trueY};
		}
		else if(probability >= 0.1 && probability < 0.1 + n_Ls1){
			int pos = rand.nextInt(neighbours.size());
			return new int[]{neighbours.get(pos).x, neighbours.get(pos).y};
		}
		else if(probability >= 0.1 + n_Ls1 && probability < 0.1 + n_Ls1 + n_Ls2){
			int pos = rand.nextInt(neighbours_L2.size());
			return new int[]{neighbours_L2.get(pos).x, neighbours_L2.get(pos).y};
		}else{
			return null;
		}
	}


	public double getCurrentProb(int x, int y) {
		double ret = 0.0;
		return ret;
	}
	
	public void update() {

		move(heading);
	
		//System.out.println("Nothing is happening, no model to go for...");
	}
	
	public class Position implements Comparable{
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
		public int compareTo(Object arg0) {
			if(((Position)arg0).x - x == 0 && ((Position)arg0).y - y == 0){
				return 0;
			}
			else if(((Position)arg0).x < x){
				return -1;
			}
			return 1;
		}
	}
}