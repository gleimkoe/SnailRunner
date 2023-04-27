#ifndef STARTSTATEMACHINE_H
#define STARTSTATEMACHINE_H

#include <map>
#include <string>

/* --Forward declaration. */
class SnailRunner;

class StartStateMachine {
public:
	
	enum class State { FAILURE, START, SUCHEN, AUSRICHTEN, AUSRICHTEN_2, AUSRICHTEN_3, LAMPE_STOP, CORRECT_TRAIL_LEFT, CORRECT_TRAIL_RIGHT, ON_TRAIL, OFF_TRAIL, STOPPING, FINAL };
	enum class Event { WALL_AHEAD, NOT_WALL_AHEAD, OFF_TRAIL, ON_TRAIL, ECKEN_CNT, IS_STOPPED, ON_GREY};

	StartStateMachine(SnailRunner* r);
	/* --Returns the current state. */
	State state() const;
	/* --Process the event using state transitions. */
	void handle(Event ev);

	/* --Start the state machine, set initial state. */
	void start();
	/* --Restart the state machine. */
	void restart();
	/* -- Return value of ecke -- */
	int ecke_cnt() { return  ecke; }

private:
	State mystate;
	SnailRunner* robot; 
	int count;
	int ecke; // zählt hoch wenn 1 Ecke erreicht

	/* -- Methods called when entering a state. */
	void onEnteringFailure();
	void onEnteringStart(); // wait
	void onEnteringSuchen(); //forward 
	void onEnteringAusrichten(); // forward(10cm), turn -90
	void onEnteringAusrichten_2();
	void onEnteringAusrichten_3();
	void onEnteringOnTrail();// forward
	void onEnteringOffTrail();// stop
	void onEnteringStopping();// ecken_cnt()
	void onEnteringCorrectTrailRight(); // turn()
	void onEnteringCorrectTrailLeft(); // turn()
	void onEnteringLampeStop();//lamp.on
	void onEnteringFinal();
	

	/* -- Methods called when in a state */
	//void inAusrichten(); // turn(-90)

	/* -- Methods called when leaving a state. */
	void onLeavingStart();
	void onLeavingSuchen(); //stop
	
	void onLeavingAusrichten(); // Stop
	void onLeavingAusrichten_2();
	void onLeavingAusrichten_3();
	
	void onLeavingOnTrail();
	void onLeavingStopping();
	void onLeavingOffTrail();
	void onLeavingFailure();
	void onLeavingLampeStop();
	
	void onLeavingCorrectTrailLeft(); // stop
	void onLeavingCorrectTrailRight();// stop
	void onLeavingFinal();
	
	/* --Sets the state explicitly. */
	void state(State s);
	/* --Process state transition; returns new state. */
	void transition(Event ev);

	/* --Constant names for debugging purpose. */
	static const std::map<State, std::string> StateDescription;
	static const std::map<Event, std::string> EventDescription;

};

#endif