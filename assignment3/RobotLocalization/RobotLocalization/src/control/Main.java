package control;

import model.DummyLocalizer;
import view.RobotLocalizationViewer;

public class Main {
	/*
	 * build your own if you like, this is just an example of how to start the viewer
	 * ...
	 */
	
	public static void main( String[] args) {
		/*
		 * generate you own localiser / estimator wrapper here to plug it into the 
		 * graphics class.
		 */
		
		int width = 4, height = 4, delay = 0;
		
		if(args.length > 0) {
			delay = Integer.parseInt(args[0]);
			width = Integer.parseInt(args[1]);
			height = Integer.parseInt(args[2]);
		}
		
		EstimatorInterface l = new DummyLocalizer(width, height, 4);

		RobotLocalizationViewer viewer = new RobotLocalizationViewer( l);

		/*
		 * this thread controls the continuous update. If it is not started, 
		 * you can only click through your localisation stepwise
		 */
		new LocalizationDriver( delay, viewer).start();
		//new LocalizationDriver(0, viewer).start();
	}
}	