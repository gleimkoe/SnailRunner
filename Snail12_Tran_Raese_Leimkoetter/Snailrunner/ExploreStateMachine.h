#ifndef EXPLORESTATEMACHINE_H
#define EXPLORESTATEMACHINE_H

#include <map>
#include <string>

/* --Forward declaration. */
class SnailRunner;

/* -- Implements tracking in a single Statemachine */

class ExploreStateMachine {
public:

	enum class State { FAILURE, ON_TRAIL, OFF_TRAIL, CORRECT_TRAIL_LEFT, CORRECT_TRAIL_RIGHT, STOPPING };
	enum class Event { IS_STOPPED, ON_TRAIL, OFF_TRAIL };

	ExploreStateMachine(SnailRunner* r);

	/* --Returns the current state. */
	State state() const;
	/* --Process the event using state transitions. */
	void handle(Event ev);
	
	/* --Start the state machine, set initial state. */
	void start();
	/* --Restart the state machine. */
	void restart();

private:

	State mystate;
	SnailRunner* robot;
	int count;

	/* --Methods called when entering a state. */
	void onEnteringFailure();
	void onEnteringOn_Trail();
	void onEnteringOff_Trail();
	void onEnteringCorrectTrailLeft();
	void onEnteringCorrectTrailRight();
	void onEnteringStopping();

	/* --Methods called when leaving a state. */
	void onLeavingFailure();
	void onLeavingOn_Trail();
	void onLeavingOff_Trail();
	void onLeavingCorrectTrailLeft();
	void onLeavingCorrectTrailRight();
	void onLeavingStopping();

	/* --Sets the state explicitly. */
	void state(State s);
	/* --Process state transition; returns new state. */
	void transition(Event ev);

	/* --Constant names for debugging purpose. */
	static const std::map<State, std::string> StateDescription;
	static const std::map<Event, std::string> EventDescription;

};

#endif
