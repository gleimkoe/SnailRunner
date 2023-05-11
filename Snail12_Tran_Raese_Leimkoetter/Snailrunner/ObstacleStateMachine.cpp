#include "ObstacleStateMachine.h"
#include "SnailRunner.h"

/* --Mission 2. */

/* --Initialize State Description. */

const std::map<ObstacleStateMachine::State, std::string> ObstacleStateMachine::StateDescription = {
	{ State::FAILURE, "FAILURE" },
	{ State::FORWARD, "FORWARD" },
	{ State::OBSTACLE, "OBSTACLE" },
	{ State::TURN, "TURN" },
	{ State::STOPPING, "STOPPING" },
	{ State::FINAL, "FINAL" }
};

const std::map<ObstacleStateMachine::Event, std::string> ObstacleStateMachine::EventDescription = {
	{ Event::IS_STOPPED, "IS_STOPPED" },
	{ Event::WALL_AHEAD, "WALL_AHEAD" },
	{ Event::CLEAR_VIEW, "CLEAR_VIEW" }
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
	onEnteringForward();
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
	case State::FORWARD:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingForward(); onEnteringForward(); break;
		case Event::WALL_AHEAD: onLeavingForward(); onEnteringObstacle(); break;
		case Event::CLEAR_VIEW: break;
		default: onLeavingForward(); onEnteringFailure();
		}
		break;
	case State::OBSTACLE:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingObstacle(); onEnteringTurn(); break;
		case Event::WALL_AHEAD: break;
		case Event::CLEAR_VIEW: break;
		default: onLeavingObstacle(); onEnteringFailure();
		}
		break;
	case State::TURN:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingTurn(); onEnteringFinal(); break;
		case Event::WALL_AHEAD: break;
		case Event::CLEAR_VIEW: onLeavingTurn(); onEnteringStopping(); break;
		default: onLeavingTurn(); onEnteringFailure();
		}
		break;
	case State::STOPPING:
		switch (ev) {
		case Event::IS_STOPPED:
			onLeavingStopping();
			if (++count<3)
				onEnteringForward();
			else
				onEnteringFinal();
			break;
		case Event::WALL_AHEAD: break;
		case Event::CLEAR_VIEW: break;
		default: onLeavingStopping(); onEnteringFailure();
		}
		break;
	case State::FINAL:
		switch (ev) {
		case Event::IS_STOPPED: break;
		case Event::WALL_AHEAD: break;
		case Event::CLEAR_VIEW: break;
		default:;
		}
		break;
	default: state(State::FAILURE);
	};
}

void ObstacleStateMachine::onEnteringFailure() {
	state(State::FAILURE);
	robot->lampright().on();
}

void ObstacleStateMachine::onEnteringForward() {
	state(State::FORWARD);
	robot->forward(1, METER);
}

void ObstacleStateMachine::onEnteringObstacle() {
	state(State::OBSTACLE);
	robot->lampleft().on();
	robot->stop();
}

void ObstacleStateMachine::onEnteringTurn() {
	state(State::TURN);
	robot->turnleft(3, ROTATION);
}

void ObstacleStateMachine::onEnteringStopping() {
	state(State::STOPPING);
	robot->lampleft().off();
	robot->stop();
}

void ObstacleStateMachine::onEnteringFinal() {
	state(State::FINAL);
	robot->lampfront().on();
}

void ObstacleStateMachine::onLeavingFailure() {}
void ObstacleStateMachine::onLeavingForward() {}
void ObstacleStateMachine::onLeavingObstacle() {}
void ObstacleStateMachine::onLeavingTurn() {}
void ObstacleStateMachine::onLeavingStopping() {}

