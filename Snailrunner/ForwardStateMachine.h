#ifndef FORWARDMISSION_H
#define FORWARDMISSION_H

#include <map>
#include <string>

/* --Forward declaration. */
class SnailRunner;

/* -- Implements forward movement. Robot must be placed on track. 
	When track is lost, SearchStatemachine will be started */

class ForwardStateMachine {
public:

	enum class State { FOUND, LOST, STOPPED };
	enum class Event { IS_STOPPED, OFF_TRAIL };

	ForwardStateMachine(SnailRunner* r);

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

	/* --Methods called when entering a state. */
	void onEnteringFound();
	void onEnteringLost();

	/* --Methods called when leaving a state. */
	void onLeavingLost();

	/* --Sets the state explicitly. */
	void state(State s);
	/* --Process state transition; returns new state. */
	void transition(Event ev);

	/* --Constant names for debugging purpose. */
	static const std::map<State, std::string> StateDescription;
	static const std::map<Event, std::string> EventDescription;

};

#endif
