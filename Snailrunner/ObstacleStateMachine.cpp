#include "ObstacleStateMachine.h"
#include "SnailRunner.h"

/* --Mission 2. */

/* --Initialize State Description. */



const std::map<ObstacleStateMachine::State, std::string> ObstacleStateMachine::StateDescription = {
	{ State::FAILURE, "FAILURE" },
	{ State::ON_TRAIL, "ON_TRAIL" },
	{ State::OBSTACLE_DETECTED, "OBSTACLE_DETECTED" },
	{ State::EVASION_1, "EVASION_1" },
	{ State::EVASION_2, "EVASION_2" },
	{ State::EVASION_3, "EVASION_3" },
	{ State::EVASION_4, "EVASION_4" },
	{ State::EVASION_5, "EVASION_5" },
	{ State::EVASION_6, "EVASION_6"},
	{ State::BACK_1, "BACK_1" },
	{ State::BACK_2, "BACK_2" },
	{ State::ALLIGN_1, "ALLIGN_1" },
	{ State::ALLIGN_2, "ALLIGN_2" }
};

const std::map<ObstacleStateMachine::Event, std::string> ObstacleStateMachine::EventDescription = {
	{ Event::IS_STOPPED, "IS_STOPPED" },
	{ Event::WALL_AHEAD, "WALL_AHEAD" },
	{ Event::CLEAR_VIEW, "CLEAR_VIEW" },
	{ Event::ON_TRAIL, "ON_TRAIL"},
	{ Event::NO_SIDEWALL, "NO_SIDEWALL"}
};

ObstacleStateMachine::ObstacleStateMachine(SnailRunner* r) :mystate(State::FAILURE), robot(r), count(0) {}

void ObstacleStateMachine::state(State s) {
	/* --Log state chance. Check if description is available. */
	bool isOldStateAvailable = (StateDescription.find(mystate) != StateDescription.end());
	bool isNewStateAvailable = (StateDescription.find(s) != StateDescription.end());
	/* --If program stops here, you have to add a state description to the map at top of this file !!! */
	if (!isOldStateAvailable && !isNewStateAvailable)
		OutputDebugString((LPCTSTR)"*** CHECK YOUR CODE! STATE DESCRIPTION IS MISSING ***");

	/* --Log string. */
	std::string note = std::string("OLD STATE:") + (isOldStateAvailable ? StateDescription.at(mystate) : "*ERROR*")
		+ std::string(" --> NEW STATE:") + (isNewStateAvailable ? StateDescription.at(s) : "*ERROR*");
	robot->controller()->annotate(note);

	/* --Set new state.*/
	mystate = s;
}

ObstacleStateMachine::State ObstacleStateMachine::state() const { return mystate; }

void ObstacleStateMachine::start() {
	/* --Log string. */
	robot->controller()->annotate("RE/START OBSTACLE STATE MACHINE");
	/* --Set everything to initial values. */
	count = 0;
	/* --Start with initial transition. */
	onEnteringOnTrail();
}


void ObstacleStateMachine::restart() {
	/* --Same as start. */
	start();
}

void ObstacleStateMachine::handle(Event ev) {
	State old = mystate;

	/* --Logging information. */
	bool isEventAvailable = (EventDescription.find(ev) != EventDescription.end());
	/* --If program stops here, you have to add an event description to the map at top of this file !!! */
	if (!isEventAvailable)
		OutputDebugString((LPCTSTR)"*** CHECK YOUR CODE! EVENT DESCRIPTION IS MISSING ***");

	/* --Log string. */
	std::string note = std::string("EVENT TRIGGER:") + (isEventAvailable ? EventDescription.at(ev) : "*ERROR*");
	robot->controller()->annotate(note);

	/* --Process the event. */
	transition(ev);

	/* --Check if state was changed. Could be an issue with your state machine.*/
	if (old == mystate)
		robot->controller()->annotate(std::string("MAYBE UNHANDLED TRIGGER!"));
}

void ObstacleStateMachine::transition(Event ev) {
	switch (mystate) {
	case State::ON_TRAIL:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED:
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD: onLeavingOnTrail(); onEnteringObstacleDetected();
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW:
			break;
		case ObstacleStateMachine::Event::ON_TRAIL:
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingOnTrail(); onEnteringFailure();
		}
		break;

	case State::OBSTACLE_DETECTED:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED: onLeavingObstacleDetected(); onEnteringEvasion1();
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD:
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW:
			break;
		case ObstacleStateMachine::Event::ON_TRAIL:
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingObstacleDetected(); onEnteringFailure();
			
		}
		break;

	case State::EVASION_1:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED:
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD:
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW: onLeavingEvasion1(); onEnteringEvasion2();
			break;
		case ObstacleStateMachine::Event::ON_TRAIL:
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingEvasion1(); onEnteringFailure();
			
		}
		break;

	case State::EVASION_2:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED: onLeavingEvasion2(); onEnteringEvasion3();
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD:
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW:
			break;
		case ObstacleStateMachine::Event::ON_TRAIL:
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingEvasion2(); onEnteringFailure();
			
		}
		break;

	case State::EVASION_3:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED: onLeavingEvasion3(); onEnteringEvasion4();
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD:
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW:
			break;
		case ObstacleStateMachine::Event::ON_TRAIL:
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingEvasion3(); onEnteringFailure();
			
		}
		break;

	case State::EVASION_4:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED: onLeavingEvasion4(); onEnteringEvasion5();
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD:
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW:
			break;
		case ObstacleStateMachine::Event::ON_TRAIL:
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingEvasion4(); onEnteringFailure();
			
		}
		break;

	case State::EVASION_5:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED:
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD:
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW:
			break;
		case ObstacleStateMachine::Event::ON_TRAIL:
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL: onLeavingEvasion5(); onEnteringEvasion6();
			break;
		default: onLeavingEvasion5(); onEnteringFailure();
			
		}
		break;

	case State::EVASION_6:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED: onLeavingEvasion6(); onEnteringBack1();
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD:
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW:
			break;
		case ObstacleStateMachine::Event::ON_TRAIL:
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingEvasion6(); onEnteringFailure();
		
		}
		break;


	case State::BACK_1:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED: onLeavingBack1(); onEnteringBack2();
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD:
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW:
			break;
		case ObstacleStateMachine::Event::ON_TRAIL:
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingBack1(); onEnteringFailure();
			
		}
		break;

	case State::BACK_2:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED:
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD:
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW:
			break;
		case ObstacleStateMachine::Event::ON_TRAIL: onLeavingBack2(); onEnteringAllign1();
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingBack2(); onEnteringFailure();
			
		}
		break;

	case State::ALLIGN_1:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED: onLeavingAllign1(); onEnteringAllign2();
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD:
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW:
			break;
		case ObstacleStateMachine::Event::ON_TRAIL:
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingAllign1(); onEnteringFailure();
			
		}
		break;

	case State::ALLIGN_2:
		switch (ev)
		{
		case ObstacleStateMachine::Event::IS_STOPPED:
			break;
		case ObstacleStateMachine::Event::WALL_AHEAD:
			break;
		case ObstacleStateMachine::Event::CLEAR_VIEW:
			break;
		case ObstacleStateMachine::Event::ON_TRAIL: onLeavingAllign2(); onEnteringOnTrail();
			break;
		case ObstacleStateMachine::Event::NO_SIDEWALL:
			break;
		default: onLeavingAllign2(); onEnteringFailure();
			
		}
		break;
	default: state(State::FAILURE);
	};
}

void ObstacleStateMachine::onEnteringFailure() {
	state(State::FAILURE);
	robot->stop();
	robot->lampleft().on();
}

void ObstacleStateMachine::onEnteringOnTrail() {
	state(State::ON_TRAIL);
	cout << "ON_TRAIL" << endl;
	robot->forward(1, METER);
	count = 1;
}

void ObstacleStateMachine::onEnteringObstacleDetected() {
	state(State::OBSTACLE_DETECTED);
	robot->stop();
	cout << "OBSTACLE_DETECTED" << endl;
}

void ObstacleStateMachine::onEnteringEvasion1() {
	state(State::EVASION_1);
	robot->turn(90);
	cout << "EVASION_1" << endl;
}

void ObstacleStateMachine::onEnteringEvasion2() {
	state(State::EVASION_2);
	robot->turn(40); // muss noch mal anpassen!!!
	cout << "EVASION_2" << endl;

}

void ObstacleStateMachine::onEnteringEvasion3() {
	state(State::EVASION_3);
	robot->forward(0.3, METER);
	cout << "EVASION_3" << endl;

}

void ObstacleStateMachine::onEnteringEvasion4() {
	state(State::EVASION_4);
	robot->turn(-70);
	cout << "EVASION_4" << endl;

}

void ObstacleStateMachine::onEnteringEvasion5() {
	state(State::EVASION_5);
	robot->forward(1, METER);
	cout << "EVASION_5" << endl;

}

void ObstacleStateMachine::onEnteringEvasion6() {
	state(State::EVASION_6);
	robot->forward(0.05, METER);
	cout << "EVASION_6" << endl;

}

void ObstacleStateMachine::onEnteringBack1() {
	state(State::BACK_1);
	robot->turn(-70);
	cout << "BACK_1" << endl;

}

void ObstacleStateMachine::onEnteringBack2() {
	state(State::BACK_2);
	robot->forward(1, METER);
	cout << "BACK_2" << endl;

}

void ObstacleStateMachine::onEnteringAllign1() {
	state(State::ALLIGN_1);
	robot->forward(0.1, METER);
	cout << "ALLIGN_1" << endl;

}

void ObstacleStateMachine::onEnteringAllign2() {
	state(State::ALLIGN_2);
	robot->turn(180);
	cout << "ALLIGN_2" << endl;

}

void ObstacleStateMachine::onLeavingOnTrail() {}

void ObstacleStateMachine::onLeavingObstacleDetected() {}

void ObstacleStateMachine::onLeavingEvasion1() {
	robot->stop();
}

void ObstacleStateMachine::onLeavingEvasion2() {}

void ObstacleStateMachine::onLeavingEvasion3() {}

void ObstacleStateMachine::onLeavingEvasion4() {}

void ObstacleStateMachine::onLeavingEvasion5() {
	robot->stop();
}

void ObstacleStateMachine::onLeavingEvasion6() {}

void ObstacleStateMachine::onLeavingBack1() {}

void ObstacleStateMachine::onLeavingBack2() {}

void ObstacleStateMachine::onLeavingAllign1() {}

void ObstacleStateMachine::onLeavingAllign2() {}


