#include "ExploreStateMachine.h"
#include "SnailRunner.h"

/* --Initialize State Description. */

const std::map<ExploreStateMachine::State, std::string> ExploreStateMachine::StateDescription = {
	{ State::FAILURE, "FAILURE" },
	{ State::ON_TRAIL, "ON_TRAIL" },
	{ State::OFF_TRAIL, "OFF_TRAIL" },
	{ State::CORRECT_TRAIL_LEFT, "CORRECT_TRAIL_LEFT" },
	{ State::CORRECT_TRAIL_RIGHT, "CORRECT_TRAIL_RIGHT" },
	{ State::STOPPING, "STOPPING" }
	/* --INFO: Here you should add new states for debugging purpose. */
};

const std::map<ExploreStateMachine::Event, std::string> ExploreStateMachine::EventDescription = {
	{ Event::IS_STOPPED, "IS_STOPPED" },
	{ Event::ON_TRAIL, "ON_TRAIL" },
	{ Event::OFF_TRAIL, "OFF_TRAIL" }
	/* --INFO: Here you should add new events for debugging purpose. */
};


ExploreStateMachine::ExploreStateMachine(SnailRunner* r) :mystate(State::FAILURE), robot(r), count(1) {}


/* --INFO: Do not touch this method. */
void ExploreStateMachine::state(State s) {
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

/* --INFO: Do not touch this method. */
ExploreStateMachine::State ExploreStateMachine::state() const { return mystate; }

void ExploreStateMachine::start() {
	/* --Log string. */
	robot->controller()->annotate("RE/START EXPLORE STATE MACHINE");
	/* --Set everything to initial values. */
	count = 1;
	/* --Start with initial transition. */
	onEnteringOn_Trail();
	/* --INFO: Here you can change/add initial values when state machine is started. */
}


void ExploreStateMachine::restart() {
	/* --Same as start. */
	start();
}

/* --INFO: Do not touch this method. */
void ExploreStateMachine::handle(Event ev) {
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

/* --INFO: This is the implemented state machine. Add/Modify/Enhance this method for additional functionality. */
void ExploreStateMachine::transition(Event ev) {
	/* --State machine as nested switch case. */
	switch (mystate) {
	case State::ON_TRAIL:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingOn_Trail(); onEnteringOn_Trail(); break;
		case Event::ON_TRAIL: break;
		case Event::OFF_TRAIL: onLeavingOn_Trail(); onEnteringOff_Trail(); break;
		default: onLeavingOn_Trail(); onEnteringFailure();
		}
		break;
	case State::OFF_TRAIL:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingOff_Trail(); onEnteringCorrectTrailLeft(); break;
		case Event::ON_TRAIL: break;
		case Event::OFF_TRAIL: break;
		default: onLeavingOff_Trail(); onEnteringFailure();
		}
		break;
	case State::CORRECT_TRAIL_LEFT:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingCorrectTrailLeft(); onEnteringCorrectTrailRight();  break;
		case Event::ON_TRAIL: onLeavingCorrectTrailLeft(); onEnteringStopping();  break;
		case Event::OFF_TRAIL: break;
		default: onLeavingCorrectTrailLeft(); onEnteringFailure();
		}
		break;
	case State::CORRECT_TRAIL_RIGHT:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingCorrectTrailRight(); onEnteringCorrectTrailLeft();  break;
		case Event::ON_TRAIL: onLeavingCorrectTrailRight(); onEnteringStopping();  break;
		case Event::OFF_TRAIL: break;
		default: onLeavingCorrectTrailRight(); onEnteringFailure();
		}
		break;
	case State::STOPPING:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingStopping(); onEnteringOn_Trail(); break;
		case Event::ON_TRAIL: break;
		case Event::OFF_TRAIL: break;
		default: onLeavingStopping(); onEnteringFailure();
		}
		break;
	default: state(State::FAILURE);
	};
}

/* --INFO: These methods can be changed to add/modify the given behavior. Important: The
           first instruction in all onEnteringXYZ() methods should call state(State::XYZ)
		   in order to set the new/current state. */
void ExploreStateMachine::onEnteringFailure() {
	state(State::FAILURE);
	robot->stop();
	robot->lampleft().on();
}

void ExploreStateMachine::onEnteringOn_Trail() {
	state(State::ON_TRAIL);
	robot->forward(1, METER);
	count = 1;
}

void ExploreStateMachine::onEnteringOff_Trail() {
	state(State::OFF_TRAIL);
	robot->stop();
}

void ExploreStateMachine::onEnteringCorrectTrailLeft() {
	state(State::CORRECT_TRAIL_LEFT);
	count *= 2;
	robot->turnleft(count * 15);
}

void ExploreStateMachine::onEnteringCorrectTrailRight() {
	state(State::CORRECT_TRAIL_RIGHT);
	count *= 2;
	robot->turnright(count * 15);
}

void ExploreStateMachine::onEnteringStopping() {
	state(State::STOPPING);
}

void ExploreStateMachine::onLeavingFailure() {
	robot->lampright().off();
}

void ExploreStateMachine::onLeavingOn_Trail() {}

void ExploreStateMachine::onLeavingOff_Trail() {}

void ExploreStateMachine::onLeavingStopping() {}

void ExploreStateMachine::onLeavingCorrectTrailLeft() {
	robot->stop();
}

void ExploreStateMachine::onLeavingCorrectTrailRight() {
	robot->stop();
}
