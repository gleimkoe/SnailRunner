#ifndef SEARCHSTATEMACHINE_H
#define SEARCHSTATEMACHINE_H

#include <map>
#include <string>

/* --Forward declaration. */
class SnailRunner;

/* -- Implements searching the track, after track was lost. 
	When the track is found, ForwardStateMachine will be started */

class SearchStateMachine {
public:

	enum class State { STOPPED, LOOK_LEFT, LOOK_RIGHT, STOPPING };
	enum class Event { IS_STOPPED, ON_TRAIL };

	SearchStateMachine(SnailRunner* r);

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
	void onEnteringLookLeft();
	void onEnteringLookRight();
	void onEnteringStopping();

	/* --Methods called when leaving a state. */
	void onLeavingFailure();
	void onLeavingLookLeft();
	void onLeavingLookRight();
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
