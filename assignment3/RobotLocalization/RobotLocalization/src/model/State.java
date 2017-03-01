package model;

public class State {
	private int row, col, heading;
	
	public State(int row, int col, int heading){
		this.row = row;
		this.col = col;
		this.heading = heading;
	}
	
	public int getX(){
		return row;
	}
	
	public int getY(){
		return col;
	}
	
	public int getHeading(){
		return heading;
	}

	@Override
	public boolean equals(Object o) {
		State state = (State)o;
		
		return state.row == this.row && state.col == this.col && state.heading == this.heading;
	}
}