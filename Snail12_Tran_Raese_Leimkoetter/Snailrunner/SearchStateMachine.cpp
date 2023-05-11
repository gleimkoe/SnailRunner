#include "SearchStateMachine.h"
#include "SnailRunner.h"

/* --Initialize State Description. */

const std::map<SearchStateMachine::State, std::string> SearchStateMachine::StateDescription = {
	{ State::STOPPED, "STOPPED" },
	{ State::LOOK_LEFT, "LOOK_LEFT" },
	{ State::LOOK_RIGHT, "LOOK_RIGHT" },
	{ State::STOPPING, "STOPPING" }
	/* --INFO: Here you should add new states for debugging purpose. */
};

const std::map<SearchStateMachine::Event, std::string> SearchStateMachine::EventDescription = {
	{ Event::IS_STOPPED, "IS_STOPPED" },
	{ Event::ON_TRAIL, "ON_TRAIL" }
	/* --INFO: Here you should add new events for debugging purpose. */
};


SearchStateMachine::SearchStateMachine(SnailRunner* r) : robot(r), count(1) {}


/* --INFO: Do not touch this method. */
void SearchStateMachine::state(State s) {
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
SearchStateMachine::State SearchStateMachine::state() const { return mystate; }

void SearchStateMachine::start() {
	/* --Log string. */
	robot->controller()->annotate("RE/START SEARCH STATE MACHINE");
	/* --Set everything to initial values. */
	count = 1;
	/* --Start with initial transition. */
	onEnteringLookLeft();
	/* --INFO: Here you can change/add initial values when state machine is started. */
}


void SearchStateMachine::restart() {
	/* --Same as start. */
	start();
}

/* --INFO: Do not touch this method. */
void SearchStateMachine::handle(Event ev) {
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
void SearchStateMachine::transition(Event ev) {
	/* --State machine as nested switch case. */
	switch (mystate) {
	case State::LOOK_LEFT:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingLookLeft(); onEnteringLookRight();  break;
		case Event::ON_TRAIL: onLeavingLookLeft(); onEnteringStopping();  break;
		default: break;
		}
		break;
	case State::LOOK_RIGHT:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingLookRight(); onEnteringLookLeft();  break;
		case Event::ON_TRAIL: onLeavingLookRight(); onEnteringStopping();  break;
		default: break;
		}
		break;
	case State::STOPPING:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingStopping(); break;
		case Event::ON_TRAIL: break;
		default: break;
		}
		break;
	default: break;
	};
}

/* --INFO: These methods can be changed to add/modify the given behavior. Important: The
first instruction in all onEnteringXYZ() methods should call state(State::XYZ)
in order to set the new/current state. */

void SearchStateMachine::onEnteringLookLeft() {
	state(State::LOOK_LEFT);
	count *= 2;
	robot->turnleft(count * 15);
}

void SearchStateMachine::onEnteringLookRight() {
	state(State::LOOK_RIGHT);
	count *= 2;
	robot->turnright(count * 15);
}

void SearchStateMachine::onEnteringStopping() {
	state(State::STOPPING);
}

void SearchStateMachine::onLeavingFailure() {
	robot->lampright().off();
}

void SearchStateMachine::onLeavingStopping() {
	state(State::STOPPED);
	/* activate ForwardStateMachine */
	robot->activate(SnailRunner::Mission::FORWARD_MISSION);
	/* start ForwardStateMachine -> set initial state an move robot forward */
	robot->onStart();
}

void SearchStateMachine::onLeavingLookLeft() {
	robot->stop();
}

void SearchStateMachine::onLeavingLookRight() {
	robot->stop();
}