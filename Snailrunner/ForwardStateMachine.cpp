#include "ForwardStateMachine.h"
#include "SnailRunner.h"

/* --Initialize State Description. */

const std::map<ForwardStateMachine::State, std::string> ForwardStateMachine::StateDescription = {
	{ State::FOUND, "FOUND" },
	{ State::LOST, "LOST" },
	{ State::STOPPED, "STOPPED" }
	/* --INFO: Here you should add new states for debugging purpose. */
};

const std::map<ForwardStateMachine::Event, std::string> ForwardStateMachine::EventDescription = {
	{ Event::IS_STOPPED, "IS_STOPPED" },
	{ Event::OFF_TRAIL, "OFF_TRAIL" }
	/* --INFO: Here you should add new events for debugging purpose. */
};


ForwardStateMachine::ForwardStateMachine(SnailRunner* r) : robot(r) {}


/* --INFO: Do not touch this method. */
void ForwardStateMachine::state(State s) {
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
ForwardStateMachine::State ForwardStateMachine::state() const { return mystate; }

void ForwardStateMachine::start() {
	/* --Log string. */
	robot->controller()->annotate("RE/START FORWARD STATE MACHINE");
	/* --Start with initial transition. */
	onEnteringFound();
	/* --INFO: Here you can change/add initial values when state machine is started. */
}


void ForwardStateMachine::restart() {
	/* --Same as start. */
	start();
}

/* --INFO: Do not touch this method. */
void ForwardStateMachine::handle(Event ev) {
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
void ForwardStateMachine::transition(Event ev) {
	/* --State machine as nested switch case. */
	switch (mystate) {
	case State::FOUND:
		switch (ev) {
		case Event::IS_STOPPED: onEnteringFound(); break;
		case Event::OFF_TRAIL: onEnteringLost(); break;
		default: break;
		}
		break;
	case State::LOST:
		switch (ev) {
		case Event::IS_STOPPED: onLeavingLost(); break;
		case Event::OFF_TRAIL: break;
		default: break;
		}
		break;
	case State::STOPPED:
		switch (ev) {
		case Event::IS_STOPPED: break;
		case Event::OFF_TRAIL: break;
		default: break;
		}
		break;
	default: break;
	};
}

void ForwardStateMachine::onEnteringFound() {
	state(State::FOUND);
	robot->forward(1, METER);
}

void ForwardStateMachine::onEnteringLost() {
	state(State::LOST);
	robot->stop();
}

void ForwardStateMachine::onLeavingLost() {
	state(State::STOPPED);
	/* activate SearchStateMachine */
	robot->activate(SnailRunner::Mission::SEARCH_MISSION);
	/* start SearchStateMachine -> set initial state and turn robot */
	robot->onStart();
}
