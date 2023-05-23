#ifndef MOTIONMISSION_H
#define MOTIONMISSION_H

#include <map>
#include <string>

/* --Forward declaration. */
class SnailRunner;

/* -- Implements an example for avoiding an obstacle. */

class ObstacleStateMachine {
public:

	enum class State { FAILURE, ON_TRAIL, OBSTACLE_DETECTED, EVASION_1, EVASION_2, EVASION_3, EVASION_4, EVASION_5, EVASION_6, BACK_1, BACK_2, ALLIGN_1, ALLIGN_2 };
	enum class Event { IS_STOPPED, WALL_AHEAD, CLEAR_VIEW, ON_TRAIL, NO_SIDEWALL };

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
	void onEnteringOnTrail();
	void onEnteringObstacleDetected();
	void onEnteringEvasion1();
	void onEnteringEvasion2();
	void onEnteringEvasion3();
	void onEnteringEvasion4();
	void onEnteringEvasion5();
	void onEnteringEvasion6();
	void onEnteringBack1();
	void onEnteringBack2();
	void onEnteringAllign1();
	void onEnteringAllign2();


	/* --Methods called when leaving a state. */
	void onLeavingOnTrail();
	void onLeavingObstacleDetected();
	void onLeavingEvasion1();
	void onLeavingEvasion2();
	void onLeavingEvasion3();
	void onLeavingEvasion4();
	void onLeavingEvasion5();
	void onLeavingEvasion6();
	void onLeavingBack1();
	void onLeavingBack2();
	void onLeavingAllign1();
	void onLeavingAllign2();

	/* --Sets the state explicitly. */
	void state(State s);
	/* --Process state transition; returns new state. */
	void transition(Event ev);

	/* --Constant names for debugging purpose. */
	static const std::map<State, std::string> StateDescription;
	static const std::map<Event, std::string> EventDescription;

};

#endif
