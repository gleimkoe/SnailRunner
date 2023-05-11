#ifndef MOTIONMISSION_H
#define MOTIONMISSION_H

#include <map>
#include <string>

/* --Forward declaration. */
class SnailRunner;

/* -- Implements an example for avoiding an obstacle. */

class ObstacleStateMachine {
public:

	enum class State { FAILURE, FORWARD, OBSTACLE, TURN, STOPPING, FINAL };
	enum class Event { IS_STOPPED, WALL_AHEAD, CLEAR_VIEW };

	ObstacleStateMachine(SnailRunner* r);

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
	int count;	/* counts the number of found obstacles */

	/* --Methods called when entering a state. */
	void onEnteringFailure();
	void onEnteringForward();
	void onEnteringObstacle();
	void onEnteringTurn();
	void onEnteringStopping();
	void onEnteringFinal();

	/* --Methods called when leaving a state. */
	void onLeavingFailure();
	void onLeavingForward();
	void onLeavingObstacle();
	void onLeavingTurn();
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
